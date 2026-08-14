#include "camera.h"

#include <curl/curl.h>
#include <desktop_ui.h>
#include <lvgl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mjpeg/mjpeg.h"

/**
 * Frame size announced to LVGL. The geometry has to be known up front because
 * the JPEG decoder reports it straight from the descriptor for in-memory images.
 */
#define CAMERA_FRAME_WIDTH  320
#define CAMERA_FRAME_HEIGHT 240

/** How often the newest frame is handed over to the UI. */
#define CAMERA_PUBLISH_PERIOD_MS 30

/** Pause before reconnecting after the stream dropped. */
#define CAMERA_RECONNECT_DELAY_MS 1000

/** More buffered bytes than this means we are not looking at an MJPEG stream. */
#define CAMERA_MAX_FRAME_SIZE (4u * 1024u * 1024u)

typedef struct {
	uint8_t *data;
	size_t len;
	size_t capacity;
} buffer_t;

static struct {
	char url[512];
	pthread_t thread;
	pthread_mutex_t lock;
	bool running;
	bool thread_started;

	mjpeg_parser_t parser; /**< owned by the worker thread */
	buffer_t pending;      /**< newest complete frame, guarded by `lock` */
	bool has_pending;

	buffer_t frames[2];    /**< front buffers, owned by the LVGL thread */
	lv_image_dsc_t dsc[2]; /**< descriptors pointing into `frames` */
	uint8_t front;         /**< index of the published descriptor */

	lv_timer_t *timer;
} camera;

static void *camera_task(void *arg);
static void publish_timer_cb(lv_timer_t *timer);
static void on_frame(const uint8_t *data, size_t len, void *user_data);

static bool buffer_reserve(buffer_t *buffer, size_t size)
{
	if (buffer->capacity >= size) {
		return true;
	}
	uint8_t *data = realloc(buffer->data, size);
	if (!data) {
		return false;
	}
	buffer->data = data;
	buffer->capacity = size;
	return true;
}

static void buffer_free(buffer_t *buffer)
{
	free(buffer->data);
	buffer->data = NULL;
	buffer->len = 0;
	buffer->capacity = 0;
}

bool camera_init(const char *url)
{
	if (camera.thread_started) {
		return true;
	}

	if (!url) {
		url = getenv("CAMERA_URL");
	}
	if (!url) {
		url = CAMERA_DEFAULT_URL;
	}
	strncpy(camera.url, url, sizeof(camera.url) - 1);
	camera.url[sizeof(camera.url) - 1] = '\0';

	if (curl_global_init(CURL_GLOBAL_DEFAULT) != CURLE_OK) {
		fprintf(stderr, "camera: failed to initialize curl\n");
		return false;
	}

	pthread_mutex_init(&camera.lock, NULL);
	mjpeg_parser_init(&camera.parser, CAMERA_MAX_FRAME_SIZE, on_frame, NULL);
	camera.running = true;

	camera.timer = lv_timer_create(publish_timer_cb, CAMERA_PUBLISH_PERIOD_MS, NULL);
	if (!camera.timer) {
		camera.running = false;
		return false;
	}

	if (pthread_create(&camera.thread, NULL, camera_task, NULL) != 0) {
		perror("camera: pthread_create");
		lv_timer_delete(camera.timer);
		camera.timer = NULL;
		camera.running = false;
		return false;
	}
	camera.thread_started = true;

	printf("camera: streaming from %s\n", camera.url);
	return true;
}

void camera_deinit(void)
{
	if (!camera.thread_started) {
		return;
	}

	camera.running = false;
	pthread_join(camera.thread, NULL);
	camera.thread_started = false;

	if (camera.timer) {
		lv_timer_delete(camera.timer);
		camera.timer = NULL;
	}

	/* the subject must not keep pointing at a descriptor we are about to free */
	lv_subject_set_pointer(&camera_image, NULL);

	mjpeg_parser_deinit(&camera.parser);
	buffer_free(&camera.pending);
	buffer_free(&camera.frames[0]);
	buffer_free(&camera.frames[1]);
	pthread_mutex_destroy(&camera.lock);
	curl_global_cleanup();
}

/**
 * @brief Hands a complete JPEG frame over to the LVGL thread.
 *
 * Runs in the worker thread, so it must not touch LVGL. Only the newest frame
 * is kept: if the UI cannot keep up, dropping frames is the right answer.
 */
static void on_frame(const uint8_t *data, size_t len, void *user_data)
{
	LV_UNUSED(user_data);

	pthread_mutex_lock(&camera.lock);
	if (buffer_reserve(&camera.pending, len)) {
		memcpy(camera.pending.data, data, len);
		camera.pending.len = len;
		camera.has_pending = true;
	}
	pthread_mutex_unlock(&camera.lock);
}

static size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userdata)
{
	const size_t n = size * nmemb;

	if (!mjpeg_parser_feed((mjpeg_parser_t *)userdata, ptr, n)) {
		return 0; /* a short count aborts the transfer */
	}
	return n;
}

static int progress_callback(void *userdata, curl_off_t dltotal, curl_off_t dlnow,
			     curl_off_t ultotal, curl_off_t ulnow)
{
	LV_UNUSED(userdata);
	LV_UNUSED(dltotal);
	LV_UNUSED(dlnow);
	LV_UNUSED(ultotal);
	LV_UNUSED(ulnow);

	return camera.running ? 0 : 1; /* non-zero aborts the transfer */
}

static void *camera_task(void *arg)
{
	LV_UNUSED(arg);

	CURL *curl = curl_easy_init();
	if (!curl) {
		fprintf(stderr, "camera: failed to create the curl handle\n");
		return NULL;
	}

	curl_easy_setopt(curl, CURLOPT_URL, camera.url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &camera.parser);
	curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, progress_callback);
	curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5L);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

	while (camera.running) {
		mjpeg_parser_reset(&camera.parser);

		CURLcode res = curl_easy_perform(curl);
		if (!camera.running) {
			break;
		}
		if (res != CURLE_OK && res != CURLE_ABORTED_BY_CALLBACK) {
			fprintf(stderr, "camera: %s\n", curl_easy_strerror(res));
		}

		/* the stream ended (or never started), give the server some room */
		lv_sleep_ms(CAMERA_RECONNECT_DELAY_MS);
	}

	curl_easy_cleanup(curl);
	return NULL;
}

/**
 * @brief Publishes the newest frame, runs in the LVGL thread.
 *
 * The two descriptors are used in turn: a subject only notifies its observers
 * when the pointer actually changes, and the buffer LVGL currently draws from
 * must stay untouched.
 */
static void publish_timer_cb(lv_timer_t *timer)
{
	LV_UNUSED(timer);

	const uint8_t next = camera.front ^ 1u;
	buffer_t *frame = &camera.frames[next];

	pthread_mutex_lock(&camera.lock);
	if (!camera.has_pending) {
		pthread_mutex_unlock(&camera.lock);
		return;
	}
	bool ok = buffer_reserve(frame, camera.pending.len);
	if (ok) {
		memcpy(frame->data, camera.pending.data, camera.pending.len);
		frame->len = camera.pending.len;
	}
	camera.has_pending = false;
	pthread_mutex_unlock(&camera.lock);

	if (!ok) {
		return;
	}

	lv_image_dsc_t *dsc = &camera.dsc[next];

	/* forget the frame this descriptor held two frames ago */
	lv_image_cache_drop(dsc);

	dsc->header.magic = LV_IMAGE_HEADER_MAGIC;
	dsc->header.cf = LV_COLOR_FORMAT_RAW;
	dsc->header.flags = 0;
	dsc->header.w = CAMERA_FRAME_WIDTH;
	dsc->header.h = CAMERA_FRAME_HEIGHT;
	dsc->header.stride = 0;
	dsc->data = frame->data;
	dsc->data_size = (uint32_t)frame->len;

	camera.front = next;
	lv_subject_set_pointer(&camera_image, dsc);
}
