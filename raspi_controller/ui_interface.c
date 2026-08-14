#include "desktop_ui.h"
#include "desktop_ui_gen.h"
#include "msgq/msgq.h"
#include "protocol/protocol.h"
#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <controller.h>

static void led_cmd_observer(lv_observer_t *observer, lv_subject_t *subject);
static void chat_send_observer(lv_observer_t *observer, lv_subject_t *subject);
static void ui_feeder(lv_timer_t *timer);
static void clock_timer_cb(lv_timer_t *timer);

static void chat_log_append(const char *line);

static msgq_t *tx;
static msgq_t *rx;

static uint32_t latest_message_id = 0;

bool ui_interface_init(msgq_t *tx_fifo, msgq_t *rx_fifo)
{

	assert(tx_fifo);
	assert(rx_fifo);
	tx = tx_fifo;
	rx = rx_fifo;

	lv_init();
	lv_display_t *disp = lv_sdl_window_create(1280, 720);
	if (!disp) {
		perror("failed to initialize display");
		return false;
	}

	lv_indev_t *mouse = lv_sdl_mouse_create();
	assert(mouse);
	lv_indev_t *kb = lv_sdl_keyboard_create();
	assert(kb);
	lv_group_t *g = lv_group_create();
	assert(g);

	lv_group_set_default(g);
	lv_indev_set_group(kb, g);

	desktop_ui_init(NULL);
	lv_screen_load(main_screen_create());

	lv_subject_add_observer(&cmd_led_red, led_cmd_observer, (void *)(uintptr_t)LED_RED);
	lv_subject_add_observer(&cmd_led_green, led_cmd_observer, (void *)(uintptr_t)LED_GREEN);
	lv_subject_add_observer(&cmd_led_blue, led_cmd_observer, (void *)(uintptr_t)LED_BLUE);
	lv_subject_add_observer(&cmd_chat_send, chat_send_observer, NULL);
	lv_subject_set_int(&net_connected, 0);
	lv_subject_copy_string(&net_ip, "Disconnected");

	lv_timer_create(ui_feeder, 5, NULL);
	lv_timer_t *t = lv_timer_create(clock_timer_cb, 1000, NULL);
	lv_timer_ready(t);
	return true;
}

void ui_interface_loop(controller_t *controller)
{
	while (!controller->shutdown) {
		uint32_t ms = lv_timer_handler();
		ms = LV_MAX(ms, LV_DEF_REFR_PERIOD);
		lv_sleep_ms(ms);
	}
}
static void led_cmd_observer(lv_observer_t *observer, lv_subject_t *subject)
{
	led_id_t id = (led_id_t)(uintptr_t)lv_observer_get_user_data(observer);
	protocol_message_t *message = malloc(sizeof(*message));
	message->type = PMT_LED;
	message->data.led.id = id;
	message->data.led.on = lv_subject_get_int(subject);
	msgq_push(tx, message);
}

static void chat_send_observer(lv_observer_t *observer, lv_subject_t *subject)
{
	const char *msg = lv_subject_get_string(&cmd_chat_message);
	if (!msg || msg[0] == '\0') {
		return;
	}

	protocol_message_t *message = malloc(sizeof(*message));
	if (!message) {
		return;
	}
	const size_t max_message_size = sizeof(message->data.message.data) - 1;
	message->type = PMT_CHAT_MESSAGE;
	lv_strncpy((char *)message->data.message.data, msg, max_message_size);
	message->data.message.data[max_message_size] = '\0';
	/* the server assigns the real id when it stores the message */
	message->data.message.id = 0;

	msgq_push(tx, message);
}

static void ui_feeder(lv_timer_t *timer)
{
	protocol_message_t *message;
	while ((message = msgq_try_pop(rx))) {
		lv_subject_set_int(&net_connected, 1);
		switch (message->type) {
		case PMT_CHAT_MESSAGE: {
			/* the server replays its history on connect, skip what we have */
			if (message->data.message.id <= latest_message_id) {
				break;
			}
			latest_message_id = message->data.message.id;
			chat_log_append(message->data.message.data);
			break;
		}
		case PMT_JOYSTICK: {
			LV_LOG_USER("Joystick %d %d", message->type, message->data.joystick);
			int32_t x, y;

			switch (message->data.joystick) {
			case NONE:
			case MIDDLE:
				x = y = 0;
				break;
			case UP:
				x = 0;
				y = -1;
				break;
			case DOWN:
				x = 0;
				y = 1;
				break;
			case LEFT:
				x = -1;
				y = 0;
				break;
			case RIGHT:
				x = 1;
				y = 0;
				break;
			}
			lv_subject_set_int(&joy_x, x * 50);
			lv_subject_set_int(&joy_y, y * 50);
			break;
		}
		case PMT_SENSOR:
			lv_subject_set_int(&sensor_temp_c, (int32_t)lroundf(message->data.sensor.temp_c));
			lv_subject_set_int(&sensor_temp_f, (int32_t)lroundf(message->data.sensor.temp_f));
			lv_subject_set_int(&sensor_humidity,
					   (int32_t)lroundf(message->data.sensor.humidity));
			lv_subject_copy_string(&sensor_status, "Connected");
			break;
		case PMT_PLAYER_POSITION:
			lv_subject_set_int(&player_x, message->data.player_position.x);
			lv_subject_set_int(&player_y, message->data.player_position.y);
			break;
		case PMT_IP_ADDRESS:

			lv_subject_copy_string(&net_ip, message->data.ipv4);
			break;
		case PMT_LED:
			switch (message->data.led.id) {
			case LED_RED:
				lv_subject_set_int(&led_red, message->data.led.on);
				break;
			case LED_GREEN:
				lv_subject_set_int(&led_green, message->data.led.on);
				break;
			case LED_BLUE:
				lv_subject_set_int(&led_blue, message->data.led.on);
				break;
			}
			break;
		case PMT_REQUEST:
			/* not for us*/
			break;
		}

		free(message);
	}
}

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

static void clock_timer_cb(lv_timer_t *timer)
{
	LV_UNUSED(timer);

	time_t now = time(NULL);
	struct tm tm_now;
	localtime_r(&now, &tm_now);

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
