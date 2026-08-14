/**
 * @file ui_game_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "ui_game_gen.h"
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

lv_obj_t * ui_game_create(lv_obj_t * parent)
{
    LV_TRACE_OBJ_CREATE("begin");

    static lv_style_t style_guide;

    static bool style_inited = false;

    if (!style_inited) {
        /*Init all styles*/
        lv_style_init(&style_guide);

        lv_style_set_bg_color(&style_guide, COLOR_BORDER);
        lv_style_set_bg_opa(&style_guide, (255 * 100 / 100));
        lv_style_set_border_width(&style_guide, 0);

        style_inited = true;
    }


    lv_obj_t * the_root = NULL;

    #if DESKTOP_UI_CHECK_COMPILE_TARGET(DESKTOP_UI_TARGET_ALL)
    if (desktop_ui_check_target(DESKTOP_UI_TARGET_ALL)) {
        lv_obj_t * ui_panel_0 = ui_panel_create(parent, "GAME", SPACE_MD);
        lv_obj_set_name_static(ui_panel_0, "ui_game_#");

        lv_obj_t * field = lv_obj_create(ui_panel_0);
        lv_obj_set_name(field, "field");
        lv_obj_set_width(field, lv_pct(100));
        lv_obj_set_flex_grow(field, 1);
        lv_obj_set_style_pad_all(field, 0, 0);
        lv_obj_set_style_clip_corner(field, true, 0);
        lv_obj_set_flag(field, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_set_scrollbar_mode(field, LV_SCROLLBAR_MODE_OFF);
        lv_obj_add_style(field, &style_inset, 0);
        lv_obj_t * guide_h = lv_obj_create(field);
        lv_obj_set_name(guide_h, "guide_h");
        lv_obj_set_width(guide_h, lv_pct(100));
        lv_obj_set_height(guide_h, 1);
        lv_obj_set_align(guide_h, LV_ALIGN_CENTER);
        lv_obj_add_style(guide_h, &style_guide, 0);

        lv_obj_t * guide_v = lv_obj_create(field);
        lv_obj_set_name(guide_v, "guide_v");
        lv_obj_set_width(guide_v, 1);
        lv_obj_set_height(guide_v, lv_pct(100));
        lv_obj_set_align(guide_v, LV_ALIGN_CENTER);
        lv_obj_add_style(guide_v, &style_guide, 0);

        lv_obj_t * player = lv_obj_create(field);
        lv_obj_set_name(player, "player");
        lv_obj_set_width(player, 24);
        lv_obj_set_height(player, 24);
        lv_obj_set_align(player, LV_ALIGN_TOP_LEFT);
        lv_obj_set_style_radius(player, 12, 0);
        lv_obj_set_style_bg_color(player, COLOR_ACCENT, 0);
        lv_obj_set_style_bg_opa(player, (255 * 100 / 100), 0);
        lv_obj_set_style_border_width(player, 2, 0);
        lv_obj_set_style_border_color(player, COLOR_ON_ACCENT, 0);
        lv_obj_set_style_border_opa(player, (255 * 70 / 100), 0);
        lv_obj_set_style_shadow_width(player, 20, 0);
        lv_obj_set_style_shadow_color(player, COLOR_ACCENT, 0);
        lv_obj_set_style_pad_all(player, 0, 0);
        lv_obj_set_flag(player, LV_OBJ_FLAG_SCROLLABLE, false);
        lv_obj_bind_style_prop(player, LV_STYLE_TRANSLATE_X, 0, &player_x);
        lv_obj_bind_style_prop(player, LV_STYLE_TRANSLATE_Y, 0, &player_y);

        lv_obj_t * ui_row_0 = ui_row_create(ui_panel_0, SPACE_MD, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER);
        lv_obj_t * player_label = lv_label_create(ui_row_0);
        lv_obj_set_name(player_label, "player_label");
        lv_label_bind_text(player_label, &player_name, "Player: %s");
        lv_obj_add_style(player_label, &style_label_small, 0);

        lv_obj_t * ui_row_1 = ui_row_create(ui_row_0, SPACE_LG, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_width(ui_row_1, LV_SIZE_CONTENT);
        lv_obj_t * pos_x = lv_label_create(ui_row_1);
        lv_obj_set_name(pos_x, "pos_x");
        lv_label_bind_text(pos_x, &player_x, "X %d");
        lv_obj_add_style(pos_x, &style_muted, 0);

        lv_obj_t * pos_y = lv_label_create(ui_row_1);
        lv_obj_set_name(pos_y, "pos_y");
        lv_label_bind_text(pos_y, &player_y, "Y %d");
        lv_obj_add_style(pos_y, &style_muted, 0);

        the_root = ui_panel_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

