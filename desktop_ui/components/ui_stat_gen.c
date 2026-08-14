/**
 * @file ui_stat_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "ui_stat_gen.h"
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

lv_obj_t * ui_stat_create(lv_obj_t * parent, const char * caption, lv_subject_t * subject, const char * fmt, const char * unit, lv_color_t color)
{
    LV_TRACE_OBJ_CREATE("begin");


    lv_obj_t * the_root = NULL;

    #if DESKTOP_UI_CHECK_COMPILE_TARGET(DESKTOP_UI_TARGET_ALL)
    if (desktop_ui_check_target(DESKTOP_UI_TARGET_ALL)) {
        lv_obj_t * lv_obj_0 = lv_obj_create(parent);
        lv_obj_set_name_static(lv_obj_0, "ui_stat_#");
        lv_obj_set_width(lv_obj_0, LV_SIZE_CONTENT);
        lv_obj_set_height(lv_obj_0, LV_SIZE_CONTENT);
        lv_obj_set_flex_flow(lv_obj_0, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_style_pad_all(lv_obj_0, 0, 0);
        lv_obj_set_style_pad_row(lv_obj_0, 2, 0);
        lv_obj_set_style_bg_opa(lv_obj_0, 0, 0);
        lv_obj_set_style_border_width(lv_obj_0, 0, 0);
        lv_obj_set_flag(lv_obj_0, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_set_scrollbar_mode(lv_obj_0, LV_SCROLLBAR_MODE_OFF);

        lv_obj_t * lv_label_0 = lv_label_create(lv_obj_0);
        lv_label_set_text(lv_label_0, caption);
        lv_obj_add_style(lv_label_0, &style_muted, 0);

        lv_obj_t * ui_row_0 = ui_row_create(lv_obj_0, SPACE_XS, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_END);
        lv_obj_set_width(ui_row_0, LV_SIZE_CONTENT);
        lv_obj_t * value = lv_label_create(ui_row_0);
        lv_obj_set_name(value, "value");
        lv_label_bind_text(value, subject, fmt);
        lv_obj_set_style_text_color(value, color, 0);
        lv_obj_add_style(value, &style_value, 0);

        lv_obj_t * stat_unit = lv_label_create(ui_row_0);
        lv_obj_set_name(stat_unit, "stat_unit");
        lv_label_set_text(stat_unit, unit);
        lv_obj_set_style_translate_y(stat_unit, -6, 0);
        lv_obj_add_style(stat_unit, &style_muted, 0);

        the_root = lv_obj_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

