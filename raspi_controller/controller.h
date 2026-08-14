#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "msgq/msgq.h"
#include "net/net_udp.h"
#include <stdbool.h>

typedef struct {
	msgq_t ui_tx;
	msgq_t ui_rx;
	udp_socket_t *sender_socket;
	bool shutdown;
} controller_t;

#endif /*CONTROLLER_H*/
