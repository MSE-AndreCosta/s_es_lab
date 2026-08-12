#include "msgq/msgq.h"
#include "net/net_udp.h"
#include "protocol/protocol.h"
#include "server/server.h"
#include <stdlib.h>
#include <string.h>
#include "led/led.h"

static void add_chat_message(chat_messages_t *messages, const message_t *message);
static void clean_chat_messages(chat_messages_t *messages);
static void on_request(server_t *server);

void *receiver_task(void *arg)
{
	server_t *server = (server_t *)arg;
	memset(&server->chat_messages, 0, sizeof(server->chat_messages));

	udp_socket_t *socket = udp_create_receiver(SERVER_PORT);
	if (!socket) {
		return NULL;
	}
	udp_set_timeout(socket, 1000);

	uint8_t buffer[1024];
	while (!server->shutdown) {
		ssize_t n = udp_recv_frame(socket, buffer, sizeof(buffer));
		if (n < 0) {
			return NULL;
		}
		if (n == 0) {
			continue;
		}

		protocol_message_t message;
		bool decoded = protocol_message_decode((char *)buffer, (size_t)n, &message);
		if (!decoded) {
			continue;
		}

		switch (message.type) {
		case PMT_CHAT_MESSAGE:
			add_chat_message(&server->chat_messages, &message.data.message);
			protocol_message_t *out_message = malloc(sizeof(*out_message));
			if (!out_message) {
				continue;
			}
			*out_message = message;
			msgq_push(&server->tx, out_message);
			break;
		case PMT_REQUEST: {
			on_request(server);
			break;
		}
		case PMT_LED: {
			led_set(message.data.led.id, message.data.led.on);
			protocol_message_t *out_message = malloc(sizeof(*out_message));
			if (!out_message) {
				continue;
			}
			*out_message = message;
			msgq_push(&server->tx, out_message);
			break;
		}
		default:
			continue;
		}

		protocol_message_t *msg = msgq_pop(&server->tx);
		if (!msg) {
			return NULL;
		}
		char *payload = protocol_message_encode(msg);
		udp_send_frame(socket, payload, strlen(payload));

		free(msg);
		protocol_message_delete(payload);
	}
	return NULL;
}

static void add_chat_message(chat_messages_t *messages, const message_t *message)
{
	if (messages->capacity == messages->count) {
		size_t new_capacity = messages->capacity == 0 ? 1 : messages->capacity * 2;
		message_t *new_buffer =
			realloc(messages->data, new_capacity * sizeof(*messages->data));
		if (!new_buffer) {
			return;
		}
		messages->capacity = new_capacity;
		messages->data = new_buffer;
	}

	const size_t max_message_size = sizeof(messages->data[messages->count].data) - 1;
	strncpy(messages->data[messages->count].data, message->data, max_message_size);

	messages->data[messages->count].data[max_message_size] = '\0';
	messages->data[messages->count].id = message->id;
	messages->count++;
}
static void clean_chat_messages(chat_messages_t *messages)
{
	free(messages->data);
}

static void on_request(server_t *server)
{
	int err = pthread_mutex_lock(&server->data_mutex);
	if (err) {
		return;
	}
	protocol_message_t *message = malloc(sizeof(*message));
	message->type = PMT_SENSOR;
	message->data.sensor = server->sensor_data;
	msgq_push(&server->tx, message);

	protocol_message_t *joystick = malloc(sizeof(*message));
	message->type = PMT_JOYSTICK;
	message->data.joystick = server->joystick;
	msgq_push(&server->tx, message);

	protocol_message_t *player_position = malloc(sizeof(*message));
	message->type = PMT_PLAYER_POSITION;
	message->data.player_position = server->player_position;
	msgq_push(&server->tx, message);

	pthread_mutex_unlock(&server->data_mutex);

	for (size_t i = 0; i < server->chat_messages.count; ++i) {
		message_t *chat_message = &server->chat_messages.data[i];

		protocol_message_t *message = malloc(sizeof(*message));
		message->type = PMT_CHAT_MESSAGE;
		message->data.message.id = chat_message->id;
		memcpy(message->data.message.data, chat_message->data, sizeof(chat_message->data));

		msgq_push(&server->tx, message);
	}
}
