#include <gpiod.h>
#include "joystick.h"
#include <stdio.h>
#include <stddef.h>

struct gpiod_chip *chip = NULL;
struct gpiod_line_settings *settings = NULL;
struct gpiod_line_config *line_config = NULL;
struct gpiod_request_config *request_config = NULL;
struct gpiod_line_request *button = NULL;

#define GPIO_CHIP_PATH    "/dev/gpiochip0"
#define BUTTON_CENTER_PIN 5
#define BUTTON_LEFT_PIN   6
#define BUTTON_RIGHT_PIN  13
#define BUTTON_UP_PIN     19
#define BUTTON_DOWN_PIN   26

int joystick_get_fd(void)
{
	if (!button) {
		return -1;
	}
	return gpiod_line_request_get_fd(button);
}

static joystick_t pin_to_joystick(unsigned int pin)
{
	switch (pin) {
	case BUTTON_UP_PIN:
		return UP;
	case BUTTON_DOWN_PIN:
		return DOWN;
	case BUTTON_LEFT_PIN:
		return LEFT;
	case BUTTON_RIGHT_PIN:
		return RIGHT;
	case BUTTON_CENTER_PIN:
		return MIDDLE;
	default:
		return NONE;
	}
}

// lit les events en attente, renvoie true si une direction a été pressée (rising edge)
int joystick_read_direction(joystick_t *out)
{
	struct gpiod_edge_event_buffer *buf = gpiod_edge_event_buffer_new(8);
	if (!buf) {
		return -1;
	}

	int n = gpiod_line_request_read_edge_events(button, buf, 8);
	int found = 0;
	*out = NONE;

	for (int i = 0; i < n; i++) {
		struct gpiod_edge_event *event = gpiod_edge_event_buffer_get_event(buf, i);
		if (gpiod_edge_event_get_event_type(event) == GPIOD_EDGE_EVENT_RISING_EDGE) {
			unsigned int offset = gpiod_edge_event_get_line_offset(event);
			*out = pin_to_joystick(offset);
			found = 1; // on garde le dernier "rising edge" du batch
		}
	}

	gpiod_edge_event_buffer_free(buf);
	return found;
}

int joystick_poll_direction(joystick_t *out)
{
	const joystick_t directions[] = {UP, DOWN, LEFT, RIGHT, MIDDLE};

	if (!out) {
		return -1;
	}
	*out = NONE;

	for (size_t i = 0; i < sizeof(directions) / sizeof(directions[0]); ++i) {
		bool pressed;
		if (joystick_get(directions[i], &pressed) != 0) {
			return -1;
		}
		if (pressed) {
			*out = directions[i];
			return 0;
		}
	}

	return 0;
}

int joystick_init(void)
{
	if (chip) {
		return 0;
	}

	unsigned int button_pins[] = {BUTTON_CENTER_PIN, BUTTON_LEFT_PIN, BUTTON_RIGHT_PIN,
				      BUTTON_UP_PIN, BUTTON_DOWN_PIN};
	const int num_buttons = sizeof(button_pins) / sizeof(button_pins[0]);
	chip = gpiod_chip_open(GPIO_CHIP_PATH);
	if (!chip) {
		perror("gpiod_chip_open");
		return 1;
	}

	settings = gpiod_line_settings_new();
	if (!settings) {
		perror("gpiod_line_settings_new");
		return 1;
	}

	gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_INPUT);
	gpiod_line_settings_set_active_low(settings, true);
	gpiod_line_settings_set_bias(settings, GPIOD_LINE_BIAS_PULL_UP);
	gpiod_line_settings_set_edge_detection(settings, GPIOD_LINE_EDGE_BOTH);

	line_config = gpiod_line_config_new();
	gpiod_line_config_add_line_settings(line_config, button_pins, num_buttons, settings);

	request_config = gpiod_request_config_new();
	gpiod_request_config_set_consumer(request_config, "button  ");

	button = gpiod_chip_request_lines(chip, request_config, line_config);
	if (!button) {
		perror("gpiod_chip_request_lines");
		return 1;
	}
	return 0;
}

int joystick_get(joystick_t joystick, bool *state)
{
	unsigned int pin;

	if (!button) {
		fprintf(stderr, "Joystick not initialized\n");
		return -1;
	}

	if (!state) {
		fprintf(stderr, "Invalid state pointer\n");
		return -1;
	}

	switch (joystick) {
	case UP:
		pin = BUTTON_UP_PIN;
		break;
	case DOWN:
		pin = BUTTON_DOWN_PIN;
		break;
	case LEFT:
		pin = BUTTON_LEFT_PIN;
		break;
	case RIGHT:
		pin = BUTTON_RIGHT_PIN;
		break;
	case MIDDLE:
		pin = BUTTON_CENTER_PIN;
		break;
	default:
		fprintf(stderr, "Invalid joystick ID: %d\n", joystick);
		return -1;
	}
	int value = gpiod_line_request_get_value(button, pin);
	if (value < 0) {
		perror("gpiod_line_request_get_value");
		return -1;
	}

	*state = (value == GPIOD_LINE_VALUE_ACTIVE);

	return 0;
}

int joystick_deinit(void)
{

	if (button) {
		gpiod_line_request_release(button);
		button = NULL;
	}

	if (request_config) {
		gpiod_request_config_free(request_config);
		request_config = NULL;
	}

	if (line_config) {
		gpiod_line_config_free(line_config);
		line_config = NULL;
	}

	if (settings) {
		gpiod_line_settings_free(settings);
		settings = NULL;
	}

	if (chip) {
		gpiod_chip_close(chip);
		chip = NULL;
	}

	return 0;
}
