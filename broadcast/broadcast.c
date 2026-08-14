#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "broadcast.h"
#include "net/net_udp.h"
#include "net/local_ip.h"
#include "protocol/protocol.h"
#include "server/server.h"

static char *get_broadcast_payload(void);

void *broadcast_sender_task(void *arg)
{
	server_t *server = (server_t *)arg;
	char *mqtt_server_url = NULL;

	char *payload = get_broadcast_payload();
	uint8_t sleep_count = 0;
	udp_socket_t *socket = udp_create_sender("255.255.255.255", BROADCAST_PORT);
	if (!socket) {
		return NULL;
	}
	int err = udp_set_broadcast(socket, true);
	if (err) {
		udp_delete(socket);
		return NULL;
	}

	while (!server->shutdown) {
		if (!payload) {
			payload = get_broadcast_payload();
			sleep(1);
			continue;
		}
		if (sleep_count < BROADCAST_PAYLOAD_PERIOD_S) {
			sleep_count++;
			sleep(1);
			continue;
		}
		ssize_t sent = udp_send_frame(socket, payload, strlen(payload));
		sleep_count = 0;
	}
	protocol_message_delete(payload);
	return NULL;
}

static char *get_broadcast_payload(void)
{
	char *payload = NULL;

	char *url = net_fetch_local_ip();
	if (!url) {
		return NULL;
	}
	const size_t port_len = 6; /* eg: ':12345' */
	char *url_with_port = malloc(strlen(url) + port_len + 1);
	if (!url_with_port) {
		free(url);
		return NULL;
	}
	sprintf(url_with_port, "%s:%d", url, SERVER_PORT);

	size_t out_len;

	protocol_message_t message = {.type = PMT_IP_ADDRESS};
	strncpy(message.data.ipv4, url_with_port, sizeof(message.data.ipv4) - 1);
	message.data.ipv4[sizeof(message.data.ipv4) - 1] = '\0';

	payload = protocol_message_encode(&message);
	free(url);
	free(url_with_port);

	return payload;
}
