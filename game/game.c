#include "game.h"

#define GAME_MAX_X (GAME_FIELD_WIDTH - GAME_PLAYER_SIZE)
#define GAME_MAX_Y (GAME_FIELD_HEIGHT - GAME_PLAYER_SIZE)

static int32_t clamp(int32_t value, int32_t min, int32_t max)
{
	if (value < min) {
		return min;
	}
	if (value > max) {
		return max;
	}
	return value;
}

void game_init(vector2_t *position)
{
	if (!position) {
		return;
	}
	position->x = GAME_MAX_X / 2;
	position->y = GAME_MAX_Y / 2;
}

bool game_move(vector2_t *position, joystick_t direction)
{
	if (!position) {
		return false;
	}

	vector2_t next = *position;

	switch (direction) {
	case UP:
		next.y -= GAME_STEP;
		break;
	case DOWN:
		next.y += GAME_STEP;
		break;
	case LEFT:
		next.x -= GAME_STEP;
		break;
	case RIGHT:
		next.x += GAME_STEP;
		break;
	case MIDDLE:
		game_init(&next);
		break;
	case NONE:
		return false;
	}

	next.x = clamp(next.x, 0, GAME_MAX_X);
	next.y = clamp(next.y, 0, GAME_MAX_Y);

	if (next.x == position->x && next.y == position->y) {
		return false;
	}

	*position = next;
	return true;
}
