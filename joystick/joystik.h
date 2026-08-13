#ifndef JOYSTICK_H
#define JOYSTICK_H


int joystick_init(void);
int joystick_get(joystick_t *joystick);
int joystick_deinit(void);

#endif /*JOYSTICK_H*/