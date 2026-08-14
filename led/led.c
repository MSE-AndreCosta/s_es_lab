#include <gpiod.h>
#include "led.h"
#include <stdio.h>

static struct gpiod_chip *chip;
static struct gpiod_line_settings *settings;
static struct gpiod_line_config *line_cfg;
static struct gpiod_request_config *req_cfg;
static struct gpiod_line_request *request;

#define CHIP_NAME "/dev/gpiochip0" 

#define LED_RED_PIN   (16)
#define LED_GREEN_PIN (20)
#define LED_BLUE_PIN  (21)

int led_init(void)
{
	if (chip) {
		return 0;
	}

	unsigned int led_pins[] = {LED_RED_PIN, LED_GREEN_PIN, LED_BLUE_PIN};
	const int num_leds = sizeof(led_pins) / sizeof(led_pins[0]);
	chip = gpiod_chip_open(CHIP_NAME);
	if (!chip) {
		perror("gpiod_chip_open");
		return 1;
	}

	settings = gpiod_line_settings_new();
	if (!settings) {
		perror("gpiod_line_settings_new");
		return 1;
	}

	gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_OUTPUT);
	gpiod_line_settings_set_output_value(settings, GPIOD_LINE_VALUE_INACTIVE);

	line_cfg = gpiod_line_config_new();
	gpiod_line_config_add_line_settings(line_cfg, led_pins, num_leds, settings);

	req_cfg = gpiod_request_config_new();
	gpiod_request_config_set_consumer(req_cfg, "led");

	request = gpiod_chip_request_lines(chip, req_cfg, line_cfg);
	if (!request) {
		perror("gpiod_chip_request_lines");
		return 1;
	}

	for (int i = 0; i < 3; i++) {
		led_set((led_id_t)i, false);
	}

}

int led_set(led_id_t led, bool state){
	unsigned int pin;
		switch (led) {
		case LED_RED:
			pin = LED_RED_PIN;
			break;
		case LED_GREEN:
			pin = LED_GREEN_PIN;
			break;
		case LED_BLUE:
			pin = LED_BLUE_PIN;
			break;
		default:
		printf("Invalid LED ID: %d\n", led);
			return -1;
		}
		int value = gpiod_line_request_set_value(request, pin, (enum gpiod_line_value)state);

		if(value < 0) {
			perror("gpiod_line_request_set_value");
			return -1;
		}

}

int led_get(led_id_t led, bool *state){
	unsigned int pin;
	if (!request || !state) {
		fprintf(stderr, "LED not initialized or invalid state pointer\n");
		return -1;
	}

	switch (led) {
	case LED_RED:
		pin = LED_RED_PIN;
		break;
	case LED_GREEN:
		pin = LED_GREEN_PIN;
		break;
	case LED_BLUE:
		pin = LED_BLUE_PIN;
		break;
	default:
		printf("Invalid LED ID: %d\n", led);
		return -1;
	}

	int value = gpiod_line_request_get_value(request, pin);
	if (value < 0) {
		perror("gpiod_line_request_get_value");
		return -1;
	}
	*state = (value == GPIOD_LINE_VALUE_ACTIVE);
	return 0;
}



int led_deinit(void)
{
	if(request) {
		gpiod_line_request_release(request);
		request = NULL;
	}

	if(req_cfg) {
		gpiod_request_config_free(req_cfg);
		req_cfg = NULL;
	}
	if(line_cfg) {
		gpiod_line_config_free(line_cfg);
		line_cfg = NULL;
	}
	if(settings) {
		gpiod_line_settings_free(settings);
		settings = NULL;
	}
	if(chip) {
		gpiod_chip_close(chip);
		chip = NULL;
	}

	return 0;
}
