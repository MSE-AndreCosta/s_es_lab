#ifndef GAME_H
#define GAME_H

#include "protocol/protocol.h"

/**
 * Size of the play area, in the same coordinate space as the `field` object of
 * the `ui_game` component: the position is the top-left translation of the
 * player, so the usable range is [0, GAME_FIELD_* - GAME_PLAYER_SIZE].
 */
#define GAME_FIELD_WIDTH  296
#define GAME_FIELD_HEIGHT 240
#define GAME_PLAYER_SIZE  24

/** Distance travelled per joystick step, in pixels. */
#define GAME_STEP 8

/**
 * @brief Places the player in the middle of the play area.
 * @param position  Position to initialize.
 */
void game_init(vector2_t *position);

/**
 * @brief Moves the player one step in the direction the joystick points to.
 *
 * @c MIDDLE re-centers the player, @c NONE leaves it alone. The position is
 * clamped to the play area, so pushing against a wall is a no-op.
 *
 * @param position   Position to update in place.
 * @param direction  Direction currently held on the joystick.
 *
 * @return true if @p position changed, false otherwise.
 */
bool game_move(vector2_t *position, joystick_t direction);

#endif /*GAME_H*/
