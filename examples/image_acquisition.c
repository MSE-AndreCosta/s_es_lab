#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <lvgl.h>
#include <time.h>
#include <curl/curl.h>



typedef struct {
    unsigned char *buf;
    size_t len;
    size_t cap;
    int got_frame;
} frame_ctx;
lv_obj_t* image;
lv_image_dsc_t image_dsc;

static size_t display_cap = 0;
static unsigned char *display_buf = NULL;

static size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userdata) {
    frame_ctx *ctx = (frame_ctx *)userdata;
    size_t n = size * nmemb;

    // grow buffer
    if (ctx->len + n > ctx->cap) {
        ctx->cap = (ctx->len + n) * 2;
        ctx->buf = realloc(ctx->buf, ctx->cap);
    }
    memcpy(ctx->buf + ctx->len, ptr, n);
    ctx->len += n;

    // look for JPEG start (FFD8) and end (FFD9)
    for (size_t i = 0; i + 1 < ctx->len; i++) {
        if (ctx->buf[i] == 0xFF && ctx->buf[i+1] == 0xD8) {
            for (size_t j = i + 2; j + 1 < ctx->len; j++) {
                if (ctx->buf[j] == 0xFF && ctx->buf[j+1] == 0xD9) {
                    size_t frame_len = j + 2 - i;

                    // copy into its own buffer, don't point at ctx->buf directly
                    if (frame_len > display_cap) {
                        display_cap = frame_len;
                        display_buf = realloc(display_buf, display_cap);
                    }
                    memcpy(display_buf, ctx->buf + i, frame_len);


                    // found a complete frame from i to j+1
					image_dsc.data = display_buf;
					image_dsc.data_size = display_cap;
					image_dsc.header.flags = LV_IMAGE_FLAGS_MODIFIABLE;
					image_dsc.header.cf = LV_COLOR_FORMAT_RAW;
					image_dsc.header.w = 640;
                    image_dsc.header.h = 480;
				
					lv_obj_invalidate(image);
					lv_image_set_src(image, &image_dsc);
                    ctx->got_frame = 1;
                    return 0; // returning != n tells curl to abort
                }
            }
        }
    }

    return n;
}

int main(void)
{



	lv_init();
	lv_display_t *disp = lv_sdl_window_create(1280, 720);
	if (!disp) {
		perror("failed to initialize display");
		return 1;
	}
	lv_libjpeg_turbo_init();
	lv_indev_t *kb = lv_sdl_keyboard_create();
	assert(kb);
	lv_group_t *g = lv_group_create();
	assert(g);

	lv_group_set_default(g);
	lv_indev_set_group(kb, g);
	image = lv_image_create(lv_screen_active());
	lv_obj_center(image);
	image_dsc = (lv_image_dsc_t){
		.data = NULL,
		.data_size = 0
	};
	lv_image_set_src(image, &image_dsc);

	// ↓ CURL part
printf("start getting image\n");
    CURL *curl = curl_easy_init();
    if (!curl) return 1;

    frame_ctx ctx = { malloc(65536), 0, 65536, 0 };


	struct timespec ts = { .tv_sec = 0, .tv_nsec = 1000 * 1000000L }; // 100 ms

	curl_easy_setopt(curl, CURLOPT_URL, "http://146.136.88.32:8000/stream");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ctx);

printf("image gotten\n");
	// ↑ curl part
	while (1) {
		uint32_t ms = lv_timer_handler();
		ms = LV_MAX(ms, LV_DEF_REFR_PERIOD);
		lv_sleep_ms(ms);

        ctx.len = 0;
        ctx.got_frame = 0;

		CURLcode res = curl_easy_perform(curl);
		if (res != CURLE_OK && !ctx.got_frame)
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		else if (ctx.got_frame)
			printf("Saved frame.jpg\n");

		nanosleep(&ts, NULL);

	}
	free(ctx.buf);
    free(display_buf);
    curl_easy_cleanup(curl);
	return 0;
}
