#include <gpiod.h>
#include "led.h"
#include <stdio.h>

static struct gpiod_chip *chip;
static struct gpiod_line_settings *settings;
static struct gpiod_line_config *line_cfg;
static struct gpiod_request_config *req_cfg;
static struct gpiod_line_request *request;

#define LED_RED_PIN   (16)
#define LED_GREEN_PIN (20)
#define LED_BLUE_PIN  (21)

int led_init(void)
{
	if (chip) {
		return 0;
	}

	unsigned int led_pins[] = {LED_RED_PIN, LED_GREEN_PIN, LED_BLUE_PIN};

	chip = gpiod_chip_open("todo");
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
	gpiod_line_config_add_line_settings(line_cfg, led_pins, 1, settings);

	req_cfg = gpiod_request_config_new();
	gpiod_request_config_set_consumer(req_cfg, "led");

	request = gpiod_chip_request_lines(chip, req_cfg, line_cfg);
	if (!request) {
		perror("gpiod_chip_request_lines");
		return 1;
	}
}
int led_set(led_id_t led, bool state)
{
}
int led_deinit(void)
{

	gpiod_line_request_release(request);
	gpiod_request_config_free(req_cfg);
	gpiod_line_config_free(line_cfg);
	gpiod_line_settings_free(settings);
	gpiod_chip_close(chip);

	request = NULL;
	req_cfg = NULL;
	line_cfg = NULL;
	settings = NULL;
	chip = NULL;
}
