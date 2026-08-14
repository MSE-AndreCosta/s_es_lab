#include "msgq/msgq.h"
#include "net/net_udp.h"
#include "protocol/protocol.h"
#include "server/server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "led/led.h"

static uint32_t add_chat_message(chat_messages_t *messages, const message_t *message);
static void clean_chat_messages(chat_messages_t *messages);
static void on_request(server_t *server);
static void queue_message(server_t *server, const protocol_message_t *message);

void *receiver_task(void *arg)
{
	server_t *server = (server_t *)arg;

	udp_socket_t *socket = udp_create_receiver(SERVER_PORT);
	if (!socket) {
		perror("receiver: failed to create socket");
		return NULL;
	}
	udp_set_timeout(socket, 1000);

	uint8_t buffer[1024];
	while (!server->shutdown) {
		ssize_t n = udp_recv_frame(socket, buffer, sizeof(buffer));
		if (n < 0) {
			perror("receiver: recv failed");
			continue;
		}
		if (n == 0) {
			continue;
		}

		protocol_message_t message;
		if (!protocol_message_decode((char *)buffer, (size_t)n, &message)) {
			fprintf(stderr, "receiver: failed to decode %zd bytes\n", n);
			continue;
		}

		switch (message.type) {
		case PMT_CHAT_MESSAGE: {
			/* the server owns the ids so every client agrees on the order */
			message.data.message.id =
				add_chat_message(&server->chat_messages, &message.data.message);
			queue_message(server, &message);
			break;
		}
		case PMT_REQUEST:
			on_request(server);
			break;
		case PMT_LED: {
			if (led_set(message.data.led.id, message.data.led.on) != 0) {
				break;
			}
			queue_message(server, &message);
			break;
		}
		default:
			break;
		}
	}

	clean_chat_messages(&server->chat_messages);
	udp_delete(socket);
	return NULL;
}

static void queue_message(server_t *server, const protocol_message_t *message)
{
	protocol_message_t *out_message = malloc(sizeof(*out_message));
	if (!out_message) {
		return;
	}
	*out_message = *message;
	msgq_push(&server->tx, out_message);
}

static uint32_t add_chat_message(chat_messages_t *messages, const message_t *message)
{
	if (messages->capacity == messages->count) {
		size_t new_capacity = messages->capacity == 0 ? 1 : messages->capacity * 2;
		message_t *new_buffer =
			realloc(messages->data, new_capacity * sizeof(*messages->data));
		if (!new_buffer) {
			return 0;
		}
		messages->capacity = new_capacity;
		messages->data = new_buffer;
	}

	const size_t max_message_size = sizeof(messages->data[messages->count].data) - 1;
	strncpy(messages->data[messages->count].data, message->data, max_message_size);

	messages->data[messages->count].data[max_message_size] = '\0';
	messages->data[messages->count].id = ++messages->last_id;
	messages->count++;

	return messages->last_id;
}
static void clean_chat_messages(chat_messages_t *messages)
{
	free(messages->data);
	messages->data = NULL;
	messages->count = messages->capacity = 0;
}

static void on_request(server_t *server)
{
	protocol_message_t message;

	int err = pthread_mutex_lock(&server->data_mutex);
	if (err) {
		return;
	}
	message.type = PMT_SENSOR;
	message.data.sensor = server->sensor_data;
	queue_message(server, &message);

	message.type = PMT_JOYSTICK;
	message.data.joystick = server->joystick;
	queue_message(server, &message);

	message.type = PMT_PLAYER_POSITION;
	message.data.player_position = server->player_position;
	queue_message(server, &message);

	for (int i = 0; i < 3; i++) {
		message.type = PMT_LED;
		message.data.led = server->leds[i];
		queue_message(server, &message);
	}
	pthread_mutex_unlock(&server->data_mutex);

	for (size_t i = 0; i < server->chat_messages.count; ++i) {
		const message_t *chat_message = &server->chat_messages.data[i];

		message.type = PMT_CHAT_MESSAGE;
		message.data.message = *chat_message;
		queue_message(server, &message);
	}
}
