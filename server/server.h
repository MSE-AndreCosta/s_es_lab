#ifndef SERVER_H
#define SERVER_H

#include "msgq/msgq.h"
#include "protocol/protocol.h"
#include <stdbool.h>
#include <bits/pthreadtypes.h>

#define SERVER_PORT    19050
#define BROADCAST_PORT 19048

typedef struct {
	message_t *data;
	size_t count;
	size_t capacity;
} chat_messages_t;

typedef struct {
	chat_messages_t chat_messages;
	sensor_data_t sensor_data;
	joystick_t joystick;
	vector2_t player_position;
	pthread_mutex_t data_mutex;
	msgq_t tx;
	bool shutdown;
} server_t;

#endif /*SERVER_H*/
