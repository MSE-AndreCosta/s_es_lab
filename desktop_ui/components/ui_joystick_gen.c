/**
 * @file ui_joystick_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "ui_joystick_gen.h"
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

lv_obj_t * ui_joystick_create(lv_obj_t * parent)
{
    LV_TRACE_OBJ_CREATE("begin");

    static lv_style_t style_guide;
    static lv_style_t style_axis_bar;

    static bool style_inited = false;

    if (!style_inited) {
        /*Init all styles*/
        lv_style_init(&style_guide);
        lv_style_init(&style_axis_bar);

        lv_style_set_bg_color(&style_guide, COLOR_BORDER);
        lv_style_set_bg_opa(&style_guide, (255 * 100 / 100));
        lv_style_set_border_width(&style_guide, 0);
        lv_style_set_bg_color(&style_axis_bar, COLOR_TRACK);
        lv_style_set_bg_opa(&style_axis_bar, (255 * 100 / 100));
        lv_style_set_radius(&style_axis_bar, 3);

        style_inited = true;
    }


    lv_obj_t * the_root = NULL;

    #if DESKTOP_UI_CHECK_COMPILE_TARGET(DESKTOP_UI_TARGET_ALL)
    if (desktop_ui_check_target(DESKTOP_UI_TARGET_ALL)) {
        lv_obj_t * ui_panel_0 = ui_panel_create(parent, "JOYSTICK", SPACE_MD);
        lv_obj_set_name_static(ui_panel_0, "ui_joystick_#");
        lv_obj_set_width(ui_panel_0, lv_pct(100));
        lv_obj_set_height(ui_panel_0, lv_pct(100));

        lv_obj_t * pad = lv_obj_create(ui_panel_0);
        lv_obj_set_name(pad, "pad");
        lv_obj_set_width(pad, lv_pct(100));
        lv_obj_set_height(pad, lv_pct(60));
        lv_obj_set_align(pad, LV_ALIGN_CENTER);
        lv_obj_set_style_align(pad, LV_ALIGN_CENTER, 0);
        lv_obj_set_style_radius(pad, 70, 0);
        lv_obj_set_style_pad_all(pad, 0, 0);
        lv_obj_set_flag(pad, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_set_scrollbar_mode(pad, LV_SCROLLBAR_MODE_OFF);
        lv_obj_add_style(pad, &style_inset, 0);
        lv_obj_t * guide_h = lv_obj_create(pad);
        lv_obj_set_name(guide_h, "guide_h");
        lv_obj_set_width(guide_h, lv_pct(100));
        lv_obj_set_height(guide_h, 1);
        lv_obj_set_align(guide_h, LV_ALIGN_CENTER);
        lv_obj_add_style(guide_h, &style_guide, 0);

        lv_obj_t * guide_v = lv_obj_create(pad);
        lv_obj_set_name(guide_v, "guide_v");
        lv_obj_set_width(guide_v, 1);
        lv_obj_set_height(guide_v, lv_pct(100));
        lv_obj_set_align(guide_v, LV_ALIGN_CENTER);
        lv_obj_add_style(guide_v, &style_guide, 0);

        lv_obj_t * knob = lv_obj_create(pad);
        lv_obj_set_name(knob, "knob");
        lv_obj_set_width(knob, 34);
        lv_obj_set_height(knob, 34);
        lv_obj_set_align(knob, LV_ALIGN_CENTER);
        lv_obj_set_style_radius(knob, 17, 0);
        lv_obj_set_style_bg_color(knob, COLOR_GREEN, 0);
        lv_obj_set_style_bg_opa(knob, (255 * 100 / 100), 0);
        lv_obj_set_style_border_width(knob, 0, 0);
        lv_obj_set_style_shadow_width(knob, 18, 0);
        lv_obj_set_style_shadow_color(knob, COLOR_GREEN, 0);
        lv_obj_set_style_pad_all(knob, 0, 0);
        lv_obj_set_flag(knob, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_bind_style_prop(knob, LV_STYLE_TRANSLATE_X, 0, &joy_x);
        lv_obj_bind_style_prop(knob, LV_STYLE_TRANSLATE_Y, 0, &joy_y);

        lv_obj_t * ui_row_0 = ui_row_create(ui_panel_0, SPACE_MD, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER);
        lv_obj_t * x_label = lv_label_create(ui_row_0);
        lv_obj_set_name(x_label, "x_label");
        lv_label_bind_text(x_label, &joy_x, "X %d");
        lv_obj_add_style(x_label, &style_label_small, 0);

        lv_obj_t * y_label = lv_label_create(ui_row_0);
        lv_obj_set_name(y_label, "y_label");
        lv_label_bind_text(y_label, &joy_y, "Y %d");
        lv_obj_add_style(y_label, &style_label_small, 0);

        lv_obj_t * x_bar = lv_bar_create(ui_panel_0);
        lv_obj_set_name(x_bar, "x_bar");
        lv_obj_set_width(x_bar, lv_pct(100));
        lv_obj_set_height(x_bar, 6);
        lv_bar_set_mode(x_bar, LV_BAR_MODE_SYMMETRICAL);
        lv_bar_set_min_value(x_bar, -46);
        lv_bar_set_max_value(x_bar, 46);
        lv_bar_bind_value(x_bar, &joy_x);
        lv_obj_set_style_bg_color(x_bar, COLOR_GREEN, LV_PART_INDICATOR);
        lv_obj_set_style_radius(x_bar, 3, LV_PART_INDICATOR);
        lv_obj_add_style(x_bar, &style_axis_bar, 0);

        lv_obj_t * y_bar = lv_bar_create(ui_panel_0);
        lv_obj_set_name(y_bar, "y_bar");
        lv_obj_set_width(y_bar, lv_pct(100));
        lv_obj_set_height(y_bar, 6);
        lv_bar_set_mode(y_bar, LV_BAR_MODE_SYMMETRICAL);
        lv_bar_set_min_value(y_bar, -46);
        lv_bar_set_max_value(y_bar, 46);
        lv_bar_bind_value(y_bar, &joy_y);
        lv_obj_set_style_bg_color(y_bar, COLOR_GREEN, LV_PART_INDICATOR);
        lv_obj_set_style_radius(y_bar, 3, LV_PART_INDICATOR);
        lv_obj_add_style(y_bar, &style_axis_bar, 0);

        the_root = ui_panel_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

