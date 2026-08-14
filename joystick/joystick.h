#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "protocol/protocol.h"


int joystick_init(void);
int joystick_get(joystick_t joystick, bool *state);
int joystick_deinit(void);
int joystick_get_fd(void);
int joystick_read_direction(joystick_t *out);


#endif /*JOYSTICK_H*/