#include "msgq/msgq.h"
#include "net/net_udp.h"
#include "protocol/protocol.h"
#include "server/server.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "broadcast/broadcast.h"

void *transmitter_task(void *arg)
{
	server_t *server = (server_t *)arg;

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
		protocol_message_t *msg = msgq_pop(&server->tx);
		if (!msg) {
			return NULL;
		}
		char *payload = protocol_message_encode(msg);
		if(!payload){
		    perror("failed to encode message");
			free(msg);
			continue;
		}
		udp_send_frame(socket, payload, strlen(payload));

		free(msg);
		protocol_message_delete(payload);
	}
	return NULL;
}
