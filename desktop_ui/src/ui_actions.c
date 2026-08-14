/**
 * @file ui_actions.c
 */

#include "ui_actions.h"
#include "../desktop_ui.h"

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void ui_chat_send(lv_event_t *e)
{
	lv_obj_t *btn = lv_event_get_target_obj(e);
	lv_obj_t *ta = lv_obj_find_by_name(lv_obj_get_screen(btn), "chat_input");
	if (ta == NULL) {
		LV_LOG_WARN("chat_input not found");
		return;
	}

	const char *msg = lv_textarea_get_text(ta);
	if (msg == NULL || msg[0] == '\0') {
		return;
	}

	lv_subject_copy_string(&cmd_chat_message, msg);
	lv_subject_set_int(&cmd_chat_send, lv_subject_get_int(&cmd_chat_send) + 1);

	lv_textarea_set_text(ta, "");
}
