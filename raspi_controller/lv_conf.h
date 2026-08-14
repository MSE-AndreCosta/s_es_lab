#ifndef LV_CONF_H
#define LV_CONF_H

#define LV_USE_STDLIB_MALLOC     LV_STDLIB_CLIB
#define LV_USE_OS                LV_OS_PTHREAD
#define LV_DRAW_SW_DRAW_UNIT_CNT 4

#define LV_USE_LOG    1
#define LV_LOG_LEVEL  LV_LOG_LEVEL_WARN
#define LV_LOG_PRINTF 1

#define LV_USE_SDL         1
#define LV_SDL_DIRECT_EXIT 0

#define LV_USE_OBJ_NAME 1

#define LV_BUILD_DEMOS    0
#define LV_BUILD_EXAMPLES 0
#endif /*LV_CONF_H*/
