#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "protocol/protocol.h"


int joystick_init(void);
int joystick_get(joystick_t joystick, bool *state);
int joystick_deinit(void);
int joystick_get_fd(void);
int joystick_read_direction(joystick_t *out);

/**
 * @brief Reads the direction currently held on the joystick.
 *
 * Unlike @ref joystick_read_direction this does not consume edge events, it
 * samples the line values, so it also reports a direction that is kept pressed.
 *
 * @param out  Receives the held direction, @c NONE if no button is down.
 * @return 0 on success, -1 on error.
 */
int joystick_poll_direction(joystick_t *out);


#endif /*JOYSTICK_H*/