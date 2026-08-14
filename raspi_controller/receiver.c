#include "broadcast/broadcast.h"
#include "net/net_udp.h"
#include "protocol/protocol.h"
#include "raspi_controller/controller.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void *sender_task(void *arg);
static inline void encode_and_send(udp_socket_t *socket, protocol_message_t *message);

static bool create_sender(controller_t *controller, protocol_message_t *message, pthread_t *thread)
{

	assert(message->type == PMT_IP_ADDRESS);

	char *port_start = strchr(message->data.ipv4, ':');
	if (!port_start) {
		return false;
	}

	*port_start = '\0';
	uint16_t port = atoi(port_start + 1);
	const char *ip = message->data.ipv4;

	controller->sender_socket = udp_create_sender(ip, port);
	if (!controller->sender_socket) {
		perror("failed to create sender socket");
		return false;
	}
	printf("Found server at %s:%d\n", ip, port);
	pthread_create(thread, NULL, sender_task, controller);

	return true;
}

bool receiver_init(controller_t *controller)
{

	controller->receiver_socket = udp_create_receiver(BROADCAST_PORT);

	if (!controller->receiver_socket) {
		perror("failed to create receiver socket");
		return false;
	}

	udp_set_timeout(controller->receiver_socket, 1000);
	return true;
}
void *receiver_task(void *arg)
{
	controller_t *controller = (controller_t *)arg;

	char buffer[1024];
	protocol_message_t *message = NULL;
	pthread_t sender_thread;
	bool has_sender_thread = false;

	while (!controller->shutdown) {
		ssize_t n = udp_recv_frame(controller->receiver_socket, &buffer, sizeof(buffer));

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
		printf("Message %d\n", message->type);
		msgq_push(&controller->ui_rx, message);
	}

	if (has_sender_thread) {
		msgq_push(&controller->ui_tx, NULL);
		pthread_join(sender_thread, NULL);
	}
	udp_delete(controller->receiver_socket);
	return NULL;
}

void *sender_task(void *arg)
{
	controller_t *controller = (controller_t *)arg;
	protocol_message_t *message = NULL;
	bool sender_created = false;
	protocol_message_t request_message = {.type = PMT_REQUEST};

	encode_and_send(controller->sender_socket, &request_message);
	while (!controller->shutdown) {
		protocol_message_t *message = msgq_pop(&controller->ui_tx);
		if (!message) {
			break;
		}
		encode_and_send(controller->sender_socket, message);

		free(message);
	}

	udp_delete(controller->sender_socket);
	return NULL;
}

static inline void encode_and_send(udp_socket_t *socket, protocol_message_t *message)
{

	char *payload = protocol_message_encode(message);

	if (!payload) {
		free(message);
		return;
	}
	const size_t payload_size = strlen(payload) + 1;
	udp_send_frame(socket, payload, payload_size);
}
