/**
 * @file ui_clock_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "ui_clock_gen.h"
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

lv_obj_t * ui_clock_create(lv_obj_t * parent)
{
    LV_TRACE_OBJ_CREATE("begin");

    static lv_style_t style_hand;

    static bool style_inited = false;

    if (!style_inited) {
        /*Init all styles*/
        lv_style_init(&style_hand);

        lv_style_set_bg_opa(&style_hand, (255 * 100 / 100));
        lv_style_set_border_width(&style_hand, 0);

        style_inited = true;
    }


    lv_obj_t * the_root = NULL;

    #if DESKTOP_UI_CHECK_COMPILE_TARGET(DESKTOP_UI_TARGET_ALL)
    if (desktop_ui_check_target(DESKTOP_UI_TARGET_ALL)) {
        lv_obj_t * ui_panel_0 = ui_panel_create(parent, "CLOCK", SPACE_MD);
        lv_obj_set_name_static(ui_panel_0, "ui_clock_#");
        lv_obj_set_width(ui_panel_0, lv_pct(100));
        lv_obj_set_height(ui_panel_0, lv_pct(100));

        lv_obj_t * ui_row_0 = ui_row_create(ui_panel_0, SPACE_XL, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_flex_grow(ui_row_0, 1);
        lv_obj_t * dial = lv_obj_create(ui_row_0);
        lv_obj_set_name(dial, "dial");
        lv_obj_set_width(dial, 228);
        lv_obj_set_height(dial, 228);
        lv_obj_set_style_radius(dial, 114, 0);
        lv_obj_set_style_pad_all(dial, 0, 0);
        lv_obj_set_flag(dial, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_set_scrollbar_mode(dial, LV_SCROLLBAR_MODE_OFF);
        lv_obj_add_style(dial, &style_inset, 0);
        lv_obj_t * ticks = lv_scale_create(dial);
        lv_obj_set_name(ticks, "ticks");
        lv_obj_set_width(ticks, 228);
        lv_obj_set_height(ticks, 228);
        lv_obj_set_align(ticks, LV_ALIGN_CENTER);
        lv_scale_set_mode(ticks, LV_SCALE_MODE_ROUND_INNER);
        lv_scale_set_angle_range(ticks, 360);
        lv_scale_set_rotation(ticks, 270);
        lv_scale_set_min_value(ticks, 0);
        lv_scale_set_max_value(ticks, 12);
        lv_scale_set_total_tick_count(ticks, 61);
        lv_scale_set_major_tick_every(ticks, 5);
        lv_scale_set_label_show(ticks, false);
        lv_obj_set_style_arc_opa(ticks, 0, 0);
        lv_obj_set_style_length(ticks, 5, LV_PART_ITEMS);
        lv_obj_set_style_line_width(ticks, 2, LV_PART_ITEMS);
        lv_obj_set_style_line_color(ticks, COLOR_TRACK, LV_PART_ITEMS);
        lv_obj_set_style_length(ticks, 11, LV_PART_INDICATOR);
        lv_obj_set_style_line_width(ticks, 3, LV_PART_INDICATOR);
        lv_obj_set_style_line_color(ticks, COLOR_MUTED, LV_PART_INDICATOR);

        lv_obj_t * hand_hour = lv_obj_create(dial);
        lv_obj_set_name(hand_hour, "hand_hour");
        lv_obj_set_width(hand_hour, 8);
        lv_obj_set_height(hand_hour, 62);
        lv_obj_set_align(hand_hour, LV_ALIGN_CENTER);
        lv_obj_set_y(hand_hour, -31);
        lv_obj_set_style_transform_pivot_x(hand_hour, 4, 0);
        lv_obj_set_style_transform_pivot_y(hand_hour, 62, 0);
        lv_obj_set_style_radius(hand_hour, 4, 0);
        lv_obj_set_style_bg_color(hand_hour, COLOR_TEXT, 0);
        lv_obj_add_style(hand_hour, &style_hand, 0);
        lv_obj_bind_style_prop(hand_hour, LV_STYLE_TRANSFORM_ROTATION, 0, &clock_hour_angle);

        lv_obj_t * hand_minute = lv_obj_create(dial);
        lv_obj_set_name(hand_minute, "hand_minute");
        lv_obj_set_width(hand_minute, 6);
        lv_obj_set_height(hand_minute, 88);
        lv_obj_set_align(hand_minute, LV_ALIGN_CENTER);
        lv_obj_set_y(hand_minute, -44);
        lv_obj_set_style_transform_pivot_x(hand_minute, 3, 0);
        lv_obj_set_style_transform_pivot_y(hand_minute, 88, 0);
        lv_obj_set_style_radius(hand_minute, 3, 0);
        lv_obj_set_style_bg_color(hand_minute, COLOR_TEXT, 0);
        lv_obj_add_style(hand_minute, &style_hand, 0);
        lv_obj_bind_style_prop(hand_minute, LV_STYLE_TRANSFORM_ROTATION, 0, &clock_min_angle);

        lv_obj_t * hand_second = lv_obj_create(dial);
        lv_obj_set_name(hand_second, "hand_second");
        lv_obj_set_width(hand_second, 3);
        lv_obj_set_height(hand_second, 98);
        lv_obj_set_align(hand_second, LV_ALIGN_CENTER);
        lv_obj_set_y(hand_second, -49);
        lv_obj_set_style_transform_pivot_x(hand_second, 1, 0);
        lv_obj_set_style_transform_pivot_y(hand_second, 98, 0);
        lv_obj_set_style_radius(hand_second, 2, 0);
        lv_obj_set_style_bg_color(hand_second, COLOR_RED, 0);
        lv_obj_add_style(hand_second, &style_hand, 0);
        lv_obj_bind_style_prop(hand_second, LV_STYLE_TRANSFORM_ROTATION, 0, &clock_sec_angle);

        lv_obj_t * cap = lv_obj_create(dial);
        lv_obj_set_name(cap, "cap");
        lv_obj_set_width(cap, 14);
        lv_obj_set_height(cap, 14);
        lv_obj_set_align(cap, LV_ALIGN_CENTER);
        lv_obj_set_style_radius(cap, 7, 0);
        lv_obj_set_style_bg_color(cap, COLOR_RED, 0);
        lv_obj_set_style_bg_opa(cap, (255 * 100 / 100), 0);
        lv_obj_set_style_border_width(cap, 2, 0);
        lv_obj_set_style_border_color(cap, COLOR_INSET, 0);
        lv_obj_set_style_pad_all(cap, 0, 0);

        lv_obj_t * digital = lv_obj_create(ui_row_0);
        lv_obj_set_name(digital, "digital");
        lv_obj_set_width(digital, LV_SIZE_CONTENT);
        lv_obj_set_height(digital, LV_SIZE_CONTENT);
        lv_obj_set_flex_flow(digital, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_style_pad_all(digital, 0, 0);
        lv_obj_set_style_pad_row(digital, SPACE_XS, 0);
        lv_obj_set_style_bg_opa(digital, 0, 0);
        lv_obj_set_style_border_width(digital, 0, 0);
        lv_obj_set_flag(digital, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_t * time = lv_label_create(digital);
        lv_obj_set_name(time, "time");
        lv_label_bind_text(time, &clock_time, NULL);
        lv_obj_set_width(time, 220);
        lv_obj_add_style(time, &style_clock, 0);

        lv_obj_t * date = lv_label_create(digital);
        lv_obj_set_name(date, "date");
        lv_label_bind_text(date, &clock_date, NULL);
        lv_obj_add_style(date, &style_muted, 0);

        the_root = ui_panel_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

