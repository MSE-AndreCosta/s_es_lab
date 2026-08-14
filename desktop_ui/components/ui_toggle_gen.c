/**
 * @file ui_toggle_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "ui_toggle_gen.h"
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

lv_obj_t * ui_toggle_create(lv_obj_t * parent, const char * label, lv_color_t color, lv_subject_t * subject, lv_style_t * on_style)
{
    LV_TRACE_OBJ_CREATE("begin");

    static lv_style_t style_track;

    static bool style_inited = false;

    if (!style_inited) {
        /*Init all styles*/
        lv_style_init(&style_track);

        lv_style_set_bg_color(&style_track, COLOR_TRACK);
        lv_style_set_bg_opa(&style_track, (255 * 100 / 100));

        style_inited = true;
    }


    lv_obj_t * the_root = NULL;

    #if DESKTOP_UI_CHECK_COMPILE_TARGET(DESKTOP_UI_TARGET_ALL)
    if (desktop_ui_check_target(DESKTOP_UI_TARGET_ALL)) {
        lv_obj_t * ui_row_0 = ui_row_create(parent, SPACE_MD, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_name_static(ui_row_0, "ui_toggle_#");

        lv_obj_t * ui_row_1 = ui_row_create(ui_row_0, SPACE_SM, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_width(ui_row_1, LV_SIZE_CONTENT);
        lv_obj_t * dot = lv_obj_create(ui_row_1);
        lv_obj_set_name(dot, "dot");
        lv_obj_set_width(dot, 10);
        lv_obj_set_height(dot, 10);
        lv_obj_set_style_radius(dot, 5, 0);
        lv_obj_set_style_bg_color(dot, color, 0);
        lv_obj_set_style_bg_opa(dot, (255 * 100 / 100), 0);
        lv_obj_set_style_border_width(dot, 0, 0);

        lv_obj_t * caption = lv_label_create(ui_row_1);
        lv_obj_set_name(caption, "caption");
        lv_label_set_text(caption, label);

        lv_obj_t * sw = lv_switch_create(ui_row_0);
        lv_obj_set_name(sw, "sw");
        lv_obj_set_width(sw, 52);
        lv_obj_set_height(sw, 28);
        lv_obj_bind_checked(sw, subject);
        lv_obj_add_style(sw, &style_track, LV_PART_MAIN);
        lv_obj_add_style(sw, on_style, LV_PART_INDICATOR | LV_STATE_CHECKED);

        the_root = ui_row_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

