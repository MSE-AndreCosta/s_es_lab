/**
 * @file app_stub.c
 *
 * See app_stub.h: this is a stand-in for the real application, not UI code.
 */

#include "app_stub.h"

#include "../desktop_ui.h"

#include <string.h>
#include <time.h>

/*********************
 *      DEFINES
 *********************/

#ifndef UI_SUBJECT_STRING_LENGTH
#define UI_SUBJECT_STRING_LENGTH 256
#endif

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void led_cmd_observer(lv_observer_t *observer, lv_subject_t *subject);
static void sensor_refresh_observer(lv_observer_t *observer, lv_subject_t *subject);
static void chat_send_observer(lv_observer_t *observer, lv_subject_t *subject);
static void clock_timer_cb(lv_timer_t *timer);

static void chat_log_append(const char *line);
static bool is_real_change(lv_subject_t *subject);

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void app_stub_init(void)
{
	/* Each command is owned by exactly one subscriber: this one. The UI never
	 * reads these back, and never writes the state subjects they produce. */
	lv_subject_add_observer(&cmd_led_red, led_cmd_observer, &led_red);
	lv_subject_add_observer(&cmd_led_green, led_cmd_observer, &led_green);
	lv_subject_add_observer(&cmd_led_blue, led_cmd_observer, &led_blue);

	lv_subject_add_observer(&cmd_sensor_refresh, sensor_refresh_observer, NULL);
	lv_subject_add_observer(&cmd_chat_send, chat_send_observer, NULL);

	lv_timer_t *t = lv_timer_create(clock_timer_cb, 1000, NULL);
	lv_timer_ready(t); /* show the correct time immediately, not in a second */
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**
 * `lv_subject_add_observer()` notifies once at subscribe time, so a counter
 * that only ever increments needs its first, spurious call filtered out.
 */
static bool is_real_change(lv_subject_t *subject)
{
	return lv_subject_get_int(subject) != lv_subject_get_previous_int(subject);
}

/**
 * A switch asked for a new LED state. This is where a real application would
 * drive the pin and check that it took effect; here the request is simply
 * accepted and reported back as the actual state.
 */
static void led_cmd_observer(lv_observer_t *observer, lv_subject_t *subject)
{
	lv_subject_t *state = lv_observer_get_user_data(observer);

	/* Replace with e.g. lv_gpio_write(pin, on) and report what you read back. */
	lv_subject_set_int(state, lv_subject_get_int(subject));
}

/**
 * The Refresh button was pressed.
 *
 * WARNING: the numbers below are FAKE - a deterministic walk so the bindings
 * visibly work before a sensor exists. Delete this block and publish real
 * readings; the UI needs no change.
 */
static void sensor_refresh_observer(lv_observer_t *observer, lv_subject_t *subject)
{
	LV_UNUSED(observer);
	if (!is_real_change(subject)) {
		return;
	}

	int32_t n = lv_subject_get_int(subject);
	int32_t t_c = 21 + (n * 7) % 6;  /* 21..26 C */
	int32_t rh = 40 + (n * 13) % 21; /* 40..60 % */

	lv_subject_set_int(&sensor_temp_c, t_c);
	lv_subject_set_int(&sensor_temp_f, t_c * 9 / 5 + 32);
	lv_subject_set_int(&sensor_humidity, rh);

	char buf[UI_SUBJECT_STRING_LENGTH];
	lv_snprintf(buf, sizeof(buf), "updated %s", lv_subject_get_string(&clock_time));
	lv_subject_copy_string(&sensor_status, buf);
}

/**
 * The user pressed Send. A real application would put `cmd_chat_message` on
 * the wire and only echo it into the transcript once the peer acknowledged it.
 */
static void chat_send_observer(lv_observer_t *observer, lv_subject_t *subject)
{
	LV_UNUSED(observer);
	const char *msg = lv_subject_get_string(&cmd_chat_message);
	if (!msg || msg[0] == '\0') {
		return;
	}
	char line[UI_SUBJECT_STRING_LENGTH];
	lv_snprintf(line, sizeof(line), "[%s] you: %s", lv_subject_get_string(&clock_time), msg);
	chat_log_append(line);
}

/**
 * Append one line to the transcript, dropping whole lines off the front when
 * the subject's fixed string buffer would overflow (truncating at the end
 * would cut the newest message instead).
 */
static void chat_log_append(const char *line)
{
	static char buf[UI_SUBJECT_STRING_LENGTH];

	const char *old = lv_subject_get_string(&chat_log);
	if (old == NULL) {
		old = "";
	}

	size_t line_len = strlen(line);
	if (line_len > sizeof(buf) - 1) {
		line_len = sizeof(buf) - 1;
	}

	size_t room = sizeof(buf) - 1 - line_len; /* also has to hold the '\n' */
	while (old[0] != '\0' && strlen(old) + 1 > room) {
		const char *nl = strchr(old, '\n');
		if (nl == NULL) {
			old = "";
			break;
		}
		old = nl + 1;
	}

	if (old[0] != '\0') {
		lv_snprintf(buf, sizeof(buf), "%s\n%.*s", old, (int)line_len, line);
	} else {
		lv_snprintf(buf, sizeof(buf), "%.*s", (int)line_len, line);
	}

	lv_subject_copy_string(&chat_log, buf);
}

/**
 * Publish the system time. The hands are driven by `transform_rotation`, whose
 * unit is 0.1 degrees clockwise from 12 o'clock.
 */
static void clock_timer_cb(lv_timer_t *timer)
{
	LV_UNUSED(timer);

	time_t now = time(NULL);
	struct tm tm_now;
#ifdef _WIN32
	localtime_s(&tm_now, &now);
#else
	localtime_r(&now, &tm_now);
#endif

	int h = tm_now.tm_hour;
	int m = tm_now.tm_min;
	int s = tm_now.tm_sec;

	lv_subject_set_int(&clock_hour_angle, ((h % 12) * 30 + m / 2) * 10);
	lv_subject_set_int(&clock_min_angle, (m * 6 + s / 10) * 10);
	lv_subject_set_int(&clock_sec_angle, s * 60);

	char buf[32];
	lv_snprintf(buf, sizeof(buf), "%02d:%02d:%02d", h, m, s);
	lv_subject_copy_string(&clock_time, buf);

	strftime(buf, sizeof(buf), "%a %d %b %Y", &tm_now);
	lv_subject_copy_string(&clock_date, buf);
}
