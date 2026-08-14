/**
 * @file main_screen_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "main_screen_gen.h"
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

lv_obj_t * main_screen_create(void)
{
    LV_TRACE_OBJ_CREATE("begin");


    lv_obj_t * the_root = NULL;

    #if DESKTOP_UI_CHECK_COMPILE_TARGET(DESKTOP_UI_TARGET_ALL)
    if (desktop_ui_check_target(DESKTOP_UI_TARGET_ALL)) {
        lv_obj_t * lv_obj_0 = lv_obj_create(NULL);
        lv_obj_set_name_static(lv_obj_0, "main_screen_#");
        lv_obj_set_width(lv_obj_0, lv_pct(100));
        lv_obj_set_height(lv_obj_0, lv_pct(100));
        lv_obj_set_flex_flow(lv_obj_0, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_style_pad_all(lv_obj_0, SPACE_LG, 0);
        lv_obj_set_style_pad_row(lv_obj_0, SPACE_MD, 0);
        lv_obj_set_flag(lv_obj_0, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_set_scrollbar_mode(lv_obj_0, LV_SCROLLBAR_MODE_OFF);

        lv_obj_add_style(lv_obj_0, &style_screen, 0);
        lv_obj_t * header = ui_header_create(lv_obj_0, "S_ES IOT application", "Sensor, telemetry and control dashboard");
        lv_obj_set_name(header, "header");
        lv_obj_set_width(header, lv_pct(100));
        lv_obj_set_height(header, 60);

        lv_obj_t * row_top = ui_row_create(lv_obj_0, SPACE_MD, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
        lv_obj_set_name(row_top, "row_top");
        lv_obj_set_height(row_top, 300);
        lv_obj_t * clock = ui_clock_create(row_top);
        lv_obj_set_name(clock, "clock");
        lv_obj_set_width(clock, lv_pct(40));
        lv_obj_set_height(clock, lv_pct(100));

        lv_obj_t * sensors = ui_sensors_create(row_top);
        lv_obj_set_name(sensors, "sensors");
        lv_obj_set_width(sensors, lv_pct(25));
        lv_obj_set_height(sensors, lv_pct(100));

        lv_obj_t * ui_panel_0 = ui_panel_create(row_top, "Camera", 0);
        lv_obj_set_style_flex_flow(ui_panel_0, LV_FLEX_FLOW_COLUMN, 0);
        lv_obj_set_flex_grow(ui_panel_0, 1);
        lv_obj_set_height(ui_panel_0, lv_pct(100));
        lv_obj_t * lv_obj_1 = lv_obj_create(ui_panel_0);
        lv_obj_set_flag(lv_obj_1, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_set_style_flex_flow(lv_obj_1, LV_FLEX_FLOW_ROW, 0);
        lv_obj_set_style_bg_opa(lv_obj_1, 0, 0);
        lv_obj_set_style_border_opa(lv_obj_1, 0, 0);
        lv_obj_set_width(lv_obj_1, lv_pct(100));
        lv_obj_set_height(lv_obj_1, lv_pct(100));
        lv_obj_t * lv_image_0 = lv_image_create(lv_obj_1);
        lv_image_bind_src(lv_image_0, &camera_image);
        lv_obj_set_width(lv_image_0, 320);
        lv_obj_set_height(lv_image_0, 240);
        lv_obj_set_style_radius(lv_image_0, 10, 0);

        lv_obj_t * row_bottom = ui_row_create(lv_obj_0, SPACE_MD, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
        lv_obj_set_name(row_bottom, "row_bottom");
        lv_obj_set_flex_grow(row_bottom, 1);
        lv_obj_t * game = ui_game_create(row_bottom);
        lv_obj_set_name(game, "game");
        lv_obj_set_width(game, lv_pct(25));
        lv_obj_set_height(game, lv_pct(100));

        lv_obj_t * joystick = ui_joystick_create(row_bottom);
        lv_obj_set_name(joystick, "joystick");
        lv_obj_set_width(joystick, lv_pct(20));
        lv_obj_set_height(joystick, lv_pct(100));

        lv_obj_t * chat = ui_chat_create(row_bottom);
        lv_obj_set_name(chat, "chat");
        lv_obj_set_flex_grow(chat, 1);
        lv_obj_set_height(chat, lv_pct(100));

        the_root = lv_obj_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

