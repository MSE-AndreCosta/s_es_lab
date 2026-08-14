/**
 * @file ui_led_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "ui_led_gen.h"
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

lv_obj_t * ui_led_create(lv_obj_t * parent, const char * label, lv_color_t color, lv_subject_t * subject, lv_subject_t * cmd_subject)
{
    LV_TRACE_OBJ_CREATE("begin");

    static lv_style_t style_led_off;

    static bool style_inited = false;

    if (!style_inited) {
        /*Init all styles*/
        lv_style_init(&style_led_off);

        lv_style_set_opa(&style_led_off, OPA_DIM);

        style_inited = true;
    }


    lv_obj_t * the_root = NULL;

    #if DESKTOP_UI_CHECK_COMPILE_TARGET(DESKTOP_UI_TARGET_ALL)
    if (desktop_ui_check_target(DESKTOP_UI_TARGET_ALL)) {
        lv_obj_t * lv_obj_0 = lv_obj_create(parent);
        lv_obj_set_name_static(lv_obj_0, "ui_led_#");
        lv_obj_set_width(lv_obj_0, LV_SIZE_CONTENT);
        lv_obj_set_height(lv_obj_0, LV_SIZE_CONTENT);
        lv_obj_set_flex_flow(lv_obj_0, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_style_pad_all(lv_obj_0, 0, 0);
        lv_obj_set_style_pad_row(lv_obj_0, SPACE_SM, 0);
        lv_obj_set_style_flex_cross_place(lv_obj_0, LV_FLEX_ALIGN_CENTER, 0);
        lv_obj_set_style_bg_opa(lv_obj_0, 0, 0);
        lv_obj_set_style_border_width(lv_obj_0, 0, 0);
        lv_obj_set_flag(lv_obj_0, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_set_scrollbar_mode(lv_obj_0, LV_SCROLLBAR_MODE_OFF);

        lv_obj_t * lamp = lv_led_create(lv_obj_0);
        lv_obj_set_name(lamp, "lamp");
        lv_led_set_color(lamp, color);
        lv_led_set_brightness(lamp, (255 * 100 / 100));
        lv_obj_set_width(lamp, 34);
        lv_obj_set_height(lamp, 34);
        lv_obj_set_style_shadow_width(lamp, 18, 0);
        lv_obj_set_style_shadow_spread(lamp, 2, 0);
        lv_obj_bind_style(lamp, &style_led_off, 0, subject, 0);
        lv_obj_add_subject_toggle_event(lamp, cmd_subject, LV_EVENT_CLICKED);

        lv_obj_t * name = lv_label_create(lv_obj_0);
        lv_obj_set_name(name, "name");
        lv_label_set_text(name, label);
        lv_obj_add_style(name, &style_muted, 0);

        the_root = lv_obj_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

