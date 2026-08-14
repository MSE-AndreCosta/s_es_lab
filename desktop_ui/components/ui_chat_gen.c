/**
 * @file ui_chat_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "ui_chat_gen.h"
#include "../desktop_ui.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/***********************
 *  STATIC VARIABLES
 **********************/

/***********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_obj_t * ui_chat_create(lv_obj_t * parent)
{
    LV_TRACE_OBJ_CREATE("begin");

    static lv_style_t style_input;
    static lv_style_t style_placeholder;
    static lv_style_t style_cursor;

    static bool style_inited = false;

    if (!style_inited) {
        /*Init all styles*/
        lv_style_init(&style_input);
        lv_style_init(&style_placeholder);
        lv_style_init(&style_cursor);

        lv_style_set_bg_color(&style_input, COLOR_INSET);
        lv_style_set_text_color(&style_input, COLOR_TEXT);
        lv_style_set_bg_opa(&style_input, (255 * 100 / 100));
        lv_style_set_border_color(&style_input, COLOR_BORDER);
        lv_style_set_border_width(&style_input, 1);
        lv_style_set_radius(&style_input, RADIUS_SM);
        lv_style_set_pad_hor(&style_input, SPACE_MD);
        lv_style_set_text_color(&style_placeholder, COLOR_MUTED);
        lv_style_set_bg_color(&style_cursor, COLOR_BORDER);
        lv_style_set_bg_opa(&style_cursor, (255 * 100 / 100));

        style_inited = true;
    }


    lv_obj_t * the_root = NULL;

    #if DESKTOP_UI_CHECK_COMPILE_TARGET(DESKTOP_UI_TARGET_ALL)
    if (desktop_ui_check_target(DESKTOP_UI_TARGET_ALL)) {
        lv_obj_t * ui_panel_0 = ui_panel_create(parent, "CHAT", SPACE_SM);
        lv_obj_set_name_static(ui_panel_0, "ui_chat_#");
        lv_obj_set_width(ui_panel_0, lv_pct(100));
        lv_obj_set_height(ui_panel_0, lv_pct(100));

        lv_obj_t * log = lv_obj_create(ui_panel_0);
        lv_obj_set_name(log, "log");
        lv_obj_set_width(log, lv_pct(100));
        lv_obj_set_flex_grow(log, 1);
        lv_obj_set_style_pad_all(log, SPACE_MD, 0);
        lv_obj_set_scrollbar_mode(log, LV_SCROLLBAR_MODE_AUTO);
        lv_obj_add_style(log, &style_inset, 0);
        lv_obj_add_style(log, &style_scrollbar, LV_PART_SCROLLBAR);
        lv_obj_t * log_text = lv_label_create(log);
        lv_obj_set_name(log_text, "log_text");
        lv_obj_set_width(log_text, lv_pct(100));
        lv_label_set_long_mode(log_text, LV_LABEL_LONG_MODE_WRAP);
        lv_label_bind_text(log_text, &chat_log, NULL);
        lv_obj_add_style(log_text, &style_small_body, 0);

        lv_obj_t * ui_row_0 = ui_row_create(ui_panel_0, SPACE_SM, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
        lv_obj_t * chat_input = lv_textarea_create(ui_row_0);
        lv_obj_set_name(chat_input, "chat_input");
        lv_obj_set_flex_grow(chat_input, 1);
        lv_obj_set_height(chat_input, 40);
        lv_textarea_set_one_line(chat_input, true);
        lv_textarea_set_placeholder_text(chat_input, "Write a message");
        lv_obj_add_style(chat_input, &style_input, 0);
        lv_obj_add_style(chat_input, &style_placeholder, LV_PART_TEXTAREA_PLACEHOLDER);
        lv_obj_add_style(chat_input, &style_cursor, LV_PART_CURSOR);
        lv_obj_add_event_cb(chat_input, ui_chat_send, LV_EVENT_READY, NULL);

        lv_obj_t * send = ui_button_create(ui_row_0, "Send", COLOR_ACCENT, COLOR_ON_ACCENT);
        lv_obj_set_name(send, "send");
        lv_obj_set_width(send, LV_SIZE_CONTENT);
        lv_obj_add_event_cb(send, ui_chat_send, LV_EVENT_CLICKED, NULL);

        the_root = ui_panel_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

