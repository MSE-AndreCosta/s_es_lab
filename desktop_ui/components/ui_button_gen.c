/**
 * @file ui_button_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "ui_button_gen.h"
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

lv_obj_t * ui_button_create(lv_obj_t * parent, const char * text, lv_color_t bg_color, lv_color_t text_color)
{
    LV_TRACE_OBJ_CREATE("begin");

    static lv_style_t style_btn;
    static lv_style_t style_btn_pressed;

    static bool style_inited = false;

    if (!style_inited) {
        /*Init all styles*/
        lv_style_init(&style_btn);
        lv_style_init(&style_btn_pressed);

        lv_style_set_radius(&style_btn, RADIUS_SM);
        lv_style_set_pad_hor(&style_btn, SPACE_LG);
        lv_style_set_pad_ver(&style_btn, SPACE_SM);
        lv_style_set_shadow_width(&style_btn, 0);
        lv_style_set_recolor(&style_btn_pressed, lv_color_hex(0x000000));
        lv_style_set_recolor_opa(&style_btn_pressed, (255 * 30 / 100));

        style_inited = true;
    }


    lv_obj_t * the_root = NULL;

    #if DESKTOP_UI_CHECK_COMPILE_TARGET(DESKTOP_UI_TARGET_ALL)
    if (desktop_ui_check_target(DESKTOP_UI_TARGET_ALL)) {
        lv_obj_t * lv_button_0 = lv_button_create(parent);
        lv_obj_set_name_static(lv_button_0, "ui_button_#");
        lv_obj_set_height(lv_button_0, LV_SIZE_CONTENT);
        lv_obj_set_style_bg_color(lv_button_0, bg_color, 0);
        lv_obj_set_style_text_color(lv_button_0, text_color, 0);
        lv_obj_set_style_text_font(lv_button_0, font_label, 0);

        lv_obj_add_style(lv_button_0, &style_btn, 0);
        lv_obj_add_style(lv_button_0, &style_btn_pressed, LV_STATE_PRESSED);
        lv_obj_t * caption = lv_label_create(lv_button_0);
        lv_obj_set_name(caption, "caption");
        lv_obj_set_align(caption, LV_ALIGN_CENTER);
        lv_label_set_text(caption, text);

        the_root = lv_button_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

