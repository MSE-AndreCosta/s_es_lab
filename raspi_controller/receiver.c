#include "broadcast/broadcast.h"
#include "net/net_udp.h"
#include "protocol/protocol.h"
#include "raspi_controller/controller.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void *sender_task(void *arg);

static bool create_sender(controller_t *controller, protocol_message_t *message, pthread_t *thread)
{

	assert(message->type == PMT_IP_ADDRESS);
	controller->sender_socket = udp_create_sender(message->data.ipv4, BROADCAST_PORT);
	if (!controller->sender_socket) {
		return false;
	}
	pthread_create(thread, NULL, sender_task, controller);
	return true;
}

void *receiver_task(void *arg)
{
	controller_t *controller = (controller_t *)arg;

	udp_socket_t *socket = udp_create_receiver(BROADCAST_PORT);
	if (!socket) {
		return NULL;
	}

	udp_set_timeout(socket, 1000);

	char buffer[1024];
	protocol_message_t *message = NULL;
	pthread_t sender_thread;
	bool has_sender_thread = false;

	while (!controller->shutdown) {
		ssize_t n = udp_recv_frame(socket, &buffer, sizeof(buffer));

		if (n == 0) {
			continue;
		}

		if (!message) {
			message = malloc(sizeof(*message));
		}
		if (!message) {
			continue;
		}

		bool decoded = protocol_message_decode(buffer, n, message);
		if (!decoded) {
			continue;
		}

		if (!has_sender_thread && message->type == PMT_IP_ADDRESS) {
			has_sender_thread = create_sender(controller, message, &sender_thread);
		}
	}

	if (has_sender_thread) {
		msgq_push(&controller->ui_tx, NULL);
		pthread_join(sender_thread, NULL);
	}
	return NULL;
}

void *sender_task(void *arg)
{
	controller_t *controller = (controller_t *)arg;
	protocol_message_t *message = NULL;
	bool sender_created = false;

	while (!controller->shutdown) {
		protocol_message_t *message = msgq_pop(&controller->ui_tx);
		if (!message) {
			return NULL;
		}

		char *payload = protocol_message_encode(message);

		if (!payload) {
			free(message);
			continue;
		}
		const size_t payload_size = strlen(payload) + 1;
		udp_send_frame(controller->sender_socket, payload, payload_size);
		free(message);
	}
	return NULL;
}
