
#ifndef UI_INTERFACE_H
#define UI_INTERFACE_H

#include "msgq/msgq.h"
#include "controller.h"

bool ui_interface_init(msgq_t *tx_fifo, msgq_t *rx_fifo);
void ui_interface_loop(controller_t *controller);

#endif /*UI_INTERFACE_H*/
