#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum {
	PMT_REQUEST,
	PMT_CHAT_MESSAGE,

	PMT_JOYSTICK,
	PMT_SENSOR,
	PMT_PLAYER_POSITION,
	PMT_IP_ADDRESS,
	PMT_LED,
} protocol_message_type_t;

typedef enum {
	NONE,
	UP,
	DOWN,
	LEFT,
	RIGHT,
	MIDDLE
} joystick_t;

typedef enum {
	LED_RED,
	LED_GREEN,
	LED_BLUE
} led_id_t;

typedef struct {
	led_id_t id;
	bool on;
} led_t;

typedef struct {
	double temp_c;
	double temp_f;
	double humidity;
} sensor_data_t;

typedef struct {
	int32_t x, y;
} vector2_t;

typedef struct {
	uint32_t id;
	char data[256];
} message_t;

typedef struct {
	protocol_message_type_t type;
	union {
		joystick_t joystick;
		sensor_data_t sensor;
		vector2_t player_position;
		char ipv4[32];
		led_t led;
		message_t message;
	} data;

} protocol_message_t;

bool protocol_message_decode(const char *buffer, size_t len, protocol_message_t *out_message);
char *protocol_message_encode(const protocol_message_t *message);
void protocol_message_delete(char *payload);

#endif /*PROTOCOL_H*/
