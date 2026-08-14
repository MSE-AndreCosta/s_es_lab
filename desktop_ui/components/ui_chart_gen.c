/**
 * @file ui_chart_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "ui_chart_gen.h"
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

lv_obj_t * ui_chart_create(lv_obj_t * parent, const char * title, lv_color_t color_a, lv_color_t color_b, const char * label_a, const char * label_b)
{
    LV_TRACE_OBJ_CREATE("begin");

    static lv_style_t style_grid;
    static lv_style_t style_line;
    static lv_style_t style_point;

    static bool style_inited = false;

    if (!style_inited) {
        /*Init all styles*/
        lv_style_init(&style_grid);
        lv_style_init(&style_line);
        lv_style_init(&style_point);

        lv_style_set_line_color(&style_grid, COLOR_BORDER);
        lv_style_set_line_width(&style_grid, 1);
        lv_style_set_line_dash_width(&style_grid, 2);
        lv_style_set_line_dash_gap(&style_grid, 4);
        lv_style_set_line_width(&style_line, 3);
        lv_style_set_line_rounded(&style_line, true);
        lv_style_set_width(&style_point, 7);
        lv_style_set_height(&style_point, 7);
        lv_style_set_radius(&style_point, 4);

        style_inited = true;
    }


    lv_obj_t * the_root = NULL;

    #if DESKTOP_UI_CHECK_COMPILE_TARGET(DESKTOP_UI_TARGET_ALL)
    if (desktop_ui_check_target(DESKTOP_UI_TARGET_ALL)) {
        lv_obj_t * ui_panel_0 = ui_panel_create(parent, title, SPACE_SM);
        lv_obj_set_name_static(ui_panel_0, "ui_chart_#");

        lv_obj_t * chart = lv_chart_create(ui_panel_0);
        lv_obj_set_name(chart, "chart");
        lv_obj_set_width(chart, lv_pct(100));
        lv_obj_set_flex_grow(chart, 1);
        lv_chart_set_type(chart, LV_CHART_TYPE_LINE);
        lv_chart_set_point_count(chart, 12);
        lv_chart_set_update_mode(chart, LV_CHART_UPDATE_MODE_SHIFT);
        lv_chart_set_hor_div_line_count(chart, 5);
        lv_chart_set_ver_div_line_count(chart, 7);
        lv_obj_set_style_bg_opa(chart, 0, 0);
        lv_obj_set_style_border_width(chart, 0, 0);
        lv_obj_set_style_pad_all(chart, 0, 0);
        lv_obj_set_scrollbar_mode(chart, LV_SCROLLBAR_MODE_OFF);
        lv_obj_add_style(chart, &style_grid, 0);
        lv_obj_add_style(chart, &style_line, LV_PART_ITEMS);
        lv_obj_add_style(chart, &style_point, LV_PART_INDICATOR);
        lv_chart_set_axis_min_value(chart, LV_CHART_AXIS_PRIMARY_Y, 0);
        lv_chart_set_axis_max_value(chart, LV_CHART_AXIS_PRIMARY_Y, 100);
        lv_chart_series_t * lv_chart_series_0 = lv_chart_add_series(chart, color_a, LV_CHART_AXIS_PRIMARY_Y);
        static const int32_t chart_values_0[] = {62, 64, 61, 66, 70, 73, 71, 68, 72, 76, 74, 78};
        lv_chart_set_series_values(chart, lv_chart_series_0, chart_values_0, 12);
        lv_chart_series_t * lv_chart_series_1 = lv_chart_add_series(chart, color_b, LV_CHART_AXIS_PRIMARY_Y);
        static const int32_t chart_values_1[] = {45, 47, 44, 50, 48, 52, 55, 53, 49, 46, 44, 42};
        lv_chart_set_series_values(chart, lv_chart_series_1, chart_values_1, 12);

        lv_obj_t * ui_row_0 = ui_row_create(ui_panel_0, SPACE_LG, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
        lv_obj_t * ui_row_1 = ui_row_create(ui_row_0, SPACE_SM, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_width(ui_row_1, LV_SIZE_CONTENT);
        lv_obj_t * lv_obj_0 = lv_obj_create(ui_row_1);
        lv_obj_set_width(lv_obj_0, 10);
        lv_obj_set_height(lv_obj_0, 10);
        lv_obj_set_style_radius(lv_obj_0, 5, 0);
        lv_obj_set_style_bg_color(lv_obj_0, color_a, 0);
        lv_obj_set_style_bg_opa(lv_obj_0, (255 * 100 / 100), 0);
        lv_obj_set_style_border_width(lv_obj_0, 0, 0);

        lv_obj_t * lv_label_0 = lv_label_create(ui_row_1);
        lv_label_set_text(lv_label_0, label_a);
        lv_obj_add_style(lv_label_0, &style_muted, 0);

        lv_obj_t * ui_row_2 = ui_row_create(ui_row_0, SPACE_SM, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_width(ui_row_2, LV_SIZE_CONTENT);
        lv_obj_t * lv_obj_1 = lv_obj_create(ui_row_2);
        lv_obj_set_width(lv_obj_1, 10);
        lv_obj_set_height(lv_obj_1, 10);
        lv_obj_set_style_radius(lv_obj_1, 5, 0);
        lv_obj_set_style_bg_color(lv_obj_1, color_b, 0);
        lv_obj_set_style_bg_opa(lv_obj_1, (255 * 100 / 100), 0);
        lv_obj_set_style_border_width(lv_obj_1, 0, 0);

        lv_obj_t * lv_label_1 = lv_label_create(ui_row_2);
        lv_label_set_text(lv_label_1, label_b);
        lv_obj_add_style(lv_label_1, &style_muted, 0);

        the_root = ui_panel_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

