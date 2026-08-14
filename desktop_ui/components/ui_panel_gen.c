/**
 * @file ui_panel_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "ui_panel_gen.h"
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

lv_obj_t * ui_panel_create(lv_obj_t * parent, const char * title, int32_t gap)
{
    LV_TRACE_OBJ_CREATE("begin");


    lv_obj_t * the_root = NULL;

    #if DESKTOP_UI_CHECK_COMPILE_TARGET(DESKTOP_UI_TARGET_ALL)
    if (desktop_ui_check_target(DESKTOP_UI_TARGET_ALL)) {
        lv_obj_t * lv_obj_0 = lv_obj_create(parent);
        lv_obj_set_name_static(lv_obj_0, "ui_panel_#");
        lv_obj_set_flex_flow(lv_obj_0, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_style_pad_row(lv_obj_0, gap, 0);
        lv_obj_set_flag(lv_obj_0, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_set_scrollbar_mode(lv_obj_0, LV_SCROLLBAR_MODE_OFF);

        lv_obj_add_style(lv_obj_0, &style_card, 0);
        lv_obj_t * panel_title = lv_label_create(lv_obj_0);
        lv_obj_set_name(panel_title, "panel_title");
        lv_label_set_text(panel_title, title);
        lv_obj_set_width(panel_title, lv_pct(100));
        lv_obj_add_style(panel_title, &style_section, 0);

        the_root = lv_obj_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

