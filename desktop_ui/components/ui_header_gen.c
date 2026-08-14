/**
 * @file ui_header_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "ui_header_gen.h"
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

lv_obj_t * ui_header_create(lv_obj_t * parent, const char * title, const char * subtitle)
{
    LV_TRACE_OBJ_CREATE("begin");

    static lv_style_t style_chip;
    static lv_style_t style_offline;

    static bool style_inited = false;

    if (!style_inited) {
        /*Init all styles*/
        lv_style_init(&style_chip);
        lv_style_init(&style_offline);

        lv_style_set_bg_color(&style_chip, COLOR_PANEL);
        lv_style_set_bg_opa(&style_chip, (255 * 100 / 100));
        lv_style_set_border_color(&style_chip, COLOR_BORDER);
        lv_style_set_border_width(&style_chip, 1);
        lv_style_set_radius(&style_chip, RADIUS_SM);
        lv_style_set_pad_hor(&style_chip, SPACE_MD);
        lv_style_set_pad_ver(&style_chip, SPACE_SM);
        lv_style_set_opa(&style_offline, OPA_DIM);

        style_inited = true;
    }


    lv_obj_t * the_root = NULL;

    #if DESKTOP_UI_CHECK_COMPILE_TARGET(DESKTOP_UI_TARGET_ALL)
    if (desktop_ui_check_target(DESKTOP_UI_TARGET_ALL)) {
        lv_obj_t * ui_row_0 = ui_row_create(parent, SPACE_MD, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_name_static(ui_row_0, "ui_header_#");
        lv_obj_set_height(ui_row_0, lv_pct(100));

        lv_obj_t * titles_and_logo = lv_obj_create(ui_row_0);
        lv_obj_set_name(titles_and_logo, "titles_and_logo");
        lv_obj_set_width(titles_and_logo, LV_SIZE_CONTENT);
        lv_obj_set_height(titles_and_logo, LV_SIZE_CONTENT);
        lv_obj_set_flex_flow(titles_and_logo, LV_FLEX_FLOW_ROW);
        lv_obj_set_style_pad_all(titles_and_logo, 0, 0);
        lv_obj_set_style_pad_row(titles_and_logo, 2, 0);
        lv_obj_set_style_bg_opa(titles_and_logo, 0, 0);
        lv_obj_set_style_border_width(titles_and_logo, 0, 0);
        lv_obj_set_flag(titles_and_logo, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_t * lv_image_0 = lv_image_create(titles_and_logo);
        lv_image_set_src(lv_image_0, logo);
        lv_obj_set_width(lv_image_0, 55);
        lv_obj_set_height(lv_image_0, 55);
        lv_image_set_scale_x(lv_image_0, lv_pct(90));
        lv_image_set_scale_y(lv_image_0, lv_pct(90));

        lv_obj_t * titles = lv_obj_create(titles_and_logo);
        lv_obj_set_name(titles, "titles");
        lv_obj_set_width(titles, LV_SIZE_CONTENT);
        lv_obj_set_height(titles, LV_SIZE_CONTENT);
        lv_obj_set_flex_flow(titles, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_style_pad_all(titles, 0, 0);
        lv_obj_set_style_pad_row(titles, 2, 0);
        lv_obj_set_style_bg_opa(titles, 0, 0);
        lv_obj_set_style_border_width(titles, 0, 0);
        lv_obj_set_flag(titles, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_set_style_pad_top(titles, 8, 0);
        lv_obj_t * header_title = lv_label_create(titles);
        lv_obj_set_name(header_title, "header_title");
        lv_label_set_text(header_title, title);
        lv_obj_add_style(header_title, &style_title, 0);

        lv_obj_t * header_subtitle = lv_label_create(titles);
        lv_obj_set_name(header_subtitle, "header_subtitle");
        lv_label_set_text(header_subtitle, subtitle);
        lv_obj_add_style(header_subtitle, &style_muted, 0);

        lv_obj_t * chip = lv_obj_create(ui_row_0);
        lv_obj_set_name(chip, "chip");
        lv_obj_set_width(chip, LV_SIZE_CONTENT);
        lv_obj_set_height(chip, LV_SIZE_CONTENT);
        lv_obj_set_flex_flow(chip, LV_FLEX_FLOW_ROW);
        lv_obj_set_style_flex_cross_place(chip, LV_FLEX_ALIGN_CENTER, 0);
        lv_obj_set_style_pad_column(chip, SPACE_MD, 0);
        lv_obj_set_flag(chip, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_set_scrollbar_mode(chip, LV_SCROLLBAR_MODE_OFF);
        lv_obj_add_style(chip, &style_chip, 0);
        lv_obj_t * dot = lv_obj_create(chip);
        lv_obj_set_name(dot, "dot");
        lv_obj_set_width(dot, 10);
        lv_obj_set_height(dot, 10);
        lv_obj_set_style_radius(dot, 5, 0);
        lv_obj_set_style_bg_color(dot, COLOR_GREEN, 0);
        lv_obj_set_style_bg_opa(dot, (255 * 100 / 100), 0);
        lv_obj_set_style_border_width(dot, 0, 0);
        lv_obj_set_style_shadow_width(dot, 10, 0);
        lv_obj_set_style_shadow_color(dot, COLOR_GREEN, 0);
        lv_obj_bind_style(dot, &style_offline, 0, &net_connected, 0);

        lv_obj_t * online = lv_label_create(chip);
        lv_obj_set_name(online, "online");
        lv_label_set_text(online, "Online");
        lv_obj_set_style_text_color(online, COLOR_GREEN, 0);
        lv_obj_add_style(online, &style_label_small, 0);
        lv_obj_bind_flag_if_eq(online, &net_connected, LV_OBJ_FLAG_HIDDEN, 0);

        lv_obj_t * offline = lv_label_create(chip);
        lv_obj_set_name(offline, "offline");
        lv_label_set_text(offline, "Offline");
        lv_obj_set_style_text_color(offline, COLOR_MUTED, 0);
        lv_obj_add_style(offline, &style_label_small, 0);
        lv_obj_bind_flag_if_not_eq(offline, &net_connected, LV_OBJ_FLAG_HIDDEN, 0);

        lv_obj_t * sep = lv_obj_create(chip);
        lv_obj_set_name(sep, "sep");
        lv_obj_set_width(sep, 1);
        lv_obj_set_height(sep, 18);
        lv_obj_set_style_bg_color(sep, COLOR_BORDER, 0);
        lv_obj_set_style_bg_opa(sep, (255 * 100 / 100), 0);
        lv_obj_set_style_border_width(sep, 0, 0);

        lv_obj_t * ip = lv_label_create(chip);
        lv_obj_set_name(ip, "ip");
        lv_label_bind_text(ip, &net_ip, NULL);
        lv_obj_add_style(ip, &style_label_small, 0);

        the_root = ui_row_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

