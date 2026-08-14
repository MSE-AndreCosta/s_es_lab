/**
 * @file ui_actions.h
 *
 * The only C that belongs to the UI itself.
 *
 * It exists for one reason: a button cannot read an lv_textarea's text through
 * a data binding, so the Send button needs a callback. That callback publishes
 * a COMMAND (cmd_chat_message + cmd_chat_send) and nothing else.
 *
 * Nothing here writes a STATE subject. Sensor readings, LED state, the clock
 * and the chat transcript are all produced outside the UI - see src/app_stub.c
 * for a stand-in you are meant to replace with the real application.
 */

#ifndef UI_ACTIONS_H
#define UI_ACTIONS_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

/* ---- Referenced by name from the XML; do not rename without editing it. ---- */

/**
 * "Send" button in the chat card. Moves the composer's text into
 * `cmd_chat_message`, bumps `cmd_chat_send` and clears the field.
 * It deliberately does NOT append to `chat_log`: the message shows up when the
 * application echoes it back.
 */
void ui_chat_send(lv_event_t *e);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*UI_ACTIONS_H*/
