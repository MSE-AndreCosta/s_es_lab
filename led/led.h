#ifndef LED_H
#define LED_H

#include "protocol/protocol.h"

int led_init(void);
int led_set(led_id_t led, bool state);
int led_get(led_id_t led, bool *state);
int led_deinit(void);

#endif /*LED_H*/
