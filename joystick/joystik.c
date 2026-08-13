

struct gpiod_chip *chip = NULL;
struct gpiod_line_settings *settings = NULL;
struct gpiod_line_config *line_config = NULL;
struct gpiod_request_config *request_config = NULL;
struct gpiod_line_request *button = NULL;

#define GPIO_CHIP_PATH "/dev/gpiochip0"
#define BUTTON_CENTER_PIN 5
#define BUTTON_LEFT_PIN 6
#define BUTTON_RIGHT_PIN 13
#define BUTTON_UP_PIN 19
#define BUTTON_DOWN_PIN 26

int joystick_init(void)
{
	if (chip) {
		return 0;
	}

	unsigned int button_pins[] = {BUTTON_CENTER_PIN, BUTTON_LEFT_PIN, BUTTON_RIGHT_PIN, BUTTON_UP_PIN, BUTTON_DOWN_PIN};
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

	gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_OUTPUT);
	gpiod_line_settings_set_output_value(settings, GPIOD_LINE_VALUE_INACTIVE);

	line_cfg = gpiod_line_config_new();
	gpiod_line_config_add_line_settings(line_cfg, button_pins, num_buttons, settings);

	req_cfg = gpiod_request_config_new();
	gpiod_request_config_set_consumer(req_cfg, "button  ");

	request = gpiod_chip_request_lines(chip, req_cfg, line_cfg);
	if (!request) {
		perror("gpiod_chip_request_lines");
		return 1;
	}

	for (int i = 0; i < 3; i++) {
		led_set((led_id_t)i, false);
	}

}


int joystick_get(joystick_t *joystick) {




}




int joystick_deinit(void) {




}