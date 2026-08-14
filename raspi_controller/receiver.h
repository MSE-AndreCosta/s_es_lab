#ifndef RECEIVER_H
#define RECEIVER_H

#include <stdbool.h>
#include "controller.h"
bool receiver_init(controller_t *controller);
void *receiver_task(void *arg);
#endif /*RECEIVER_H*/
