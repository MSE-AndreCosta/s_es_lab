/**
 * @file ui_sensors_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "ui_sensors_gen.h"
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

lv_obj_t * ui_sensors_create(lv_obj_t * parent)
{
    LV_TRACE_OBJ_CREATE("begin");


    lv_obj_t * the_root = NULL;

    #if DESKTOP_UI_CHECK_COMPILE_TARGET(DESKTOP_UI_TARGET_ALL)
    if (desktop_ui_check_target(DESKTOP_UI_TARGET_ALL)) {
        lv_obj_t * ui_panel_0 = ui_panel_create(parent, "ENVIRONMENT", SPACE_MD);
        lv_obj_set_name_static(ui_panel_0, "ui_sensors_#");
        lv_obj_set_width(ui_panel_0, lv_pct(100));
        lv_obj_set_height(ui_panel_0, lv_pct(100));

        lv_obj_t * ui_row_0 = ui_row_create(ui_panel_0, SPACE_LG, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
        lv_obj_t * temp_c = ui_stat_create(ui_row_0, "Temperature", &sensor_temp_c, "%d", "°C", COLOR_ACCENT);
        lv_obj_set_name(temp_c, "temp_c");
        lv_obj_set_flex_grow(temp_c, 1);

        lv_obj_t * temp_f = ui_stat_create(ui_row_0, "Fahrenheit", &sensor_temp_f, "%d", "°F", COLOR_BLUE);
        lv_obj_set_name(temp_f, "temp_f");
        lv_obj_set_flex_grow(temp_f, 1);

        lv_obj_t * humidity = ui_stat_create(ui_panel_0, "Relative humidity", &sensor_humidity, "%d", "%", COLOR_AMBER);
        lv_obj_set_name(humidity, "humidity");

        lv_obj_t * humidity_bar = lv_bar_create(ui_panel_0);
        lv_obj_set_name(humidity_bar, "humidity_bar");
        lv_obj_set_width(humidity_bar, lv_pct(100));
        lv_obj_set_height(humidity_bar, 8);
        lv_bar_set_min_value(humidity_bar, 0);
        lv_bar_set_max_value(humidity_bar, 100);
        lv_bar_bind_value(humidity_bar, &sensor_humidity);
        lv_obj_set_style_bg_color(humidity_bar, COLOR_TRACK, 0);
        lv_obj_set_style_bg_opa(humidity_bar, (255 * 100 / 100), 0);
        lv_obj_set_style_radius(humidity_bar, 4, 0);
        lv_obj_set_style_bg_color(humidity_bar, COLOR_AMBER, LV_PART_INDICATOR);
        lv_obj_set_style_radius(humidity_bar, 4, LV_PART_INDICATOR);

        lv_obj_t * spacer = lv_obj_create(ui_panel_0);
        lv_obj_set_name(spacer, "spacer");
        lv_obj_set_width(spacer, lv_pct(100));
        lv_obj_set_flex_grow(spacer, 1);
        lv_obj_set_style_bg_opa(spacer, 0, 0);
        lv_obj_set_style_border_width(spacer, 0, 0);
        lv_obj_set_style_pad_all(spacer, 0, 0);
        lv_obj_set_flag(spacer, LV_OBJ_FLAG_SCROLLABLE, false);

        lv_obj_t * ui_row_1 = ui_row_create(ui_panel_0, SPACE_MD, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER);
        lv_obj_t * lamp_red = ui_led_create(ui_row_1, "RED", COLOR_RED, &led_red, &cmd_led_red);
        lv_obj_set_name(lamp_red, "lamp_red");

        lv_obj_t * lamp_green = ui_led_create(ui_row_1, "GREEN", COLOR_GREEN, &led_green, &cmd_led_green);
        lv_obj_set_name(lamp_green, "lamp_green");

        lv_obj_t * lamp_blue = ui_led_create(ui_row_1, "BLUE", COLOR_BLUE, &led_blue, &cmd_led_blue);
        lv_obj_set_name(lamp_blue, "lamp_blue");

        lv_obj_t * status = lv_label_create(ui_panel_0);
        lv_obj_set_name(status, "status");
        lv_obj_set_width(status, lv_pct(100));
        lv_label_bind_text(status, &sensor_status, NULL);
        lv_obj_set_style_text_align(status, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_add_style(status, &style_muted, 0);

        the_root = ui_panel_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

