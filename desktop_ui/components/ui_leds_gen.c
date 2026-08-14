/**
 * @file ui_leds_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "ui_leds_gen.h"
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

lv_obj_t * ui_leds_create(lv_obj_t * parent)
{
    LV_TRACE_OBJ_CREATE("begin");

    static lv_style_t style_divider;

    static bool style_inited = false;

    if (!style_inited) {
        /*Init all styles*/
        lv_style_init(&style_divider);

        lv_style_set_bg_color(&style_divider, COLOR_BORDER);
        lv_style_set_bg_opa(&style_divider, (255 * 100 / 100));
        lv_style_set_border_width(&style_divider, 0);

        style_inited = true;
    }


    lv_obj_t * the_root = NULL;

    #if DESKTOP_UI_CHECK_COMPILE_TARGET(DESKTOP_UI_TARGET_ALL)
    if (desktop_ui_check_target(DESKTOP_UI_TARGET_ALL)) {
        lv_obj_t * ui_panel_0 = ui_panel_create(parent, "LEDS", SPACE_SM);
        lv_obj_set_name_static(ui_panel_0, "ui_leds_#");

        lv_obj_t * ui_row_0 = ui_row_create(ui_panel_0, SPACE_MD, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER);
        lv_obj_t * lamp_red = ui_led_create(ui_row_0, "RED", COLOR_RED, &led_red, &cmd_led_green);
        lv_obj_set_name(lamp_red, "lamp_red");

        lv_obj_t * lamp_green = ui_led_create(ui_row_0, "GREEN", COLOR_GREEN, &led_green, &cmd_led_green);
        lv_obj_set_name(lamp_green, "lamp_green");

        lv_obj_t * lamp_blue = ui_led_create(ui_row_0, "BLUE", COLOR_BLUE, &led_blue, &cmd_led_green);
        lv_obj_set_name(lamp_blue, "lamp_blue");

        lv_obj_t * divider = lv_obj_create(ui_panel_0);
        lv_obj_set_name(divider, "divider");
        lv_obj_set_width(divider, lv_pct(100));
        lv_obj_set_height(divider, 1);
        lv_obj_add_style(divider, &style_divider, 0);

        lv_obj_t * sw_red = ui_toggle_create(ui_panel_0, "Red", COLOR_RED, &cmd_led_red, &style_switch_red);
        lv_obj_set_name(sw_red, "sw_red");

        lv_obj_t * sw_green = ui_toggle_create(ui_panel_0, "Green", COLOR_GREEN, &cmd_led_green, &style_switch_green);
        lv_obj_set_name(sw_green, "sw_green");

        lv_obj_t * sw_blue = ui_toggle_create(ui_panel_0, "Blue", COLOR_BLUE, &cmd_led_blue, &style_switch_blue);
        lv_obj_set_name(sw_blue, "sw_blue");

        lv_obj_t * spacer = lv_obj_create(ui_panel_0);
        lv_obj_set_name(spacer, "spacer");
        lv_obj_set_width(spacer, lv_pct(100));
        lv_obj_set_flex_grow(spacer, 1);
        lv_obj_set_style_bg_opa(spacer, 0, 0);
        lv_obj_set_style_border_width(spacer, 0, 0);
        lv_obj_set_style_pad_all(spacer, 0, 0);
        lv_obj_set_flag(spacer, LV_OBJ_FLAG_SCROLLABLE, false);

        lv_obj_t * all_off = ui_button_create(ui_panel_0, "All off", COLOR_TRACK, COLOR_TEXT);
        lv_obj_set_name(all_off, "all_off");
        lv_obj_set_width(all_off, lv_pct(100));
        lv_obj_add_subject_set_int_event(all_off, &cmd_led_red, LV_EVENT_CLICKED, 0);
        lv_obj_add_subject_set_int_event(all_off, &cmd_led_green, LV_EVENT_CLICKED, 0);
        lv_obj_add_subject_set_int_event(all_off, &cmd_led_blue, LV_EVENT_CLICKED, 0);

        the_root = ui_panel_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

