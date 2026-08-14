/**
 * @file desktop_ui_gen.h
 */

#ifndef DESKTOP_UI_GEN_H
#define DESKTOP_UI_GEN_H

#ifndef UI_SUBJECT_STRING_LENGTH
#define UI_SUBJECT_STRING_LENGTH 256
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
    #include "lvgl.h"
    #include "lvgl_private.h"
#else
    #include "lvgl/lvgl.h"
    #include "lvgl/lvgl_private.h"
#endif

#if defined(LV_USE_XML) && LV_USE_XML
    #include "lv_xml/lv_xml.h"
#endif



/* Prototypes for target functions, needed by responsive const definitions */

void desktop_ui_set_target(uint32_t target);
uint32_t desktop_ui_get_target(void);
bool desktop_ui_check_target(uint32_t target);

/*********************
 *      DEFINES
 *********************/

#define DESKTOP_UI_TARGET_UNDEFINED  (0 << 1)
#define DESKTOP_UI_TARGET_TARGET1    (1 << 1)
#define DESKTOP_UI_TARGET_ALL        0x0FFFFFFF

/* By default compile for all targets, allowing to switch to any targets at runtime */
#ifndef DESKTOP_UI_COMPILE_TARGET
#define DESKTOP_UI_COMPILE_TARGET DESKTOP_UI_TARGET_ALL
#endif

#define DESKTOP_UI_CHECK_COMPILE_TARGET(target) (DESKTOP_UI_COMPILE_TARGET & (target) ? 1 : 0)

/**
 * Smallest spacing unit
 */
#define SPACE_XS 4
/**
 * Small spacing unit
 */
#define SPACE_SM 8
/**
 * Default spacing unit
 */
#define SPACE_MD 12
/**
 * Large spacing unit
 */
#define SPACE_LG 16
/**
 * Extra-large spacing unit
 */
#define SPACE_XL 24
/**
 * Radius for small controls (fields, chips)
 */
#define RADIUS_SM 8
/**
 * Radius for cards / panels
 */
#define RADIUS_LG 14
/**
 * Default hairline border width
 */
#define BORDER_WIDTH 1
/**
 * Screen background
 */
#define COLOR_BG lv_color_hex(0x0B0E14)
/**
 * Card / panel surface
 */
#define COLOR_PANEL lv_color_hex(0x151A23)
/**
 * Recessed surface: fields, play area, joystick pad
 */
#define COLOR_INSET lv_color_hex(0x0E121A)
/**
 * Hairline border between surfaces
 */
#define COLOR_BORDER lv_color_hex(0x232B38)
/**
 * Primary text
 */
#define COLOR_TEXT lv_color_hex(0xE4E9F2)
/**
 * Secondary text, captions, section headers
 */
#define COLOR_MUTED lv_color_hex(0x8A94A6)
/**
 * Primary accent / active control
 */
#define COLOR_ACCENT lv_color_hex(0x4C8DFF)
/**
 * Text drawn on the accent color
 */
#define COLOR_ON_ACCENT lv_color_hex(0xFFFFFF)
/**
 * Red LED / danger
 */
#define COLOR_RED lv_color_hex(0xFF5A5F)
/**
 * Green LED / ok / online
 */
#define COLOR_GREEN lv_color_hex(0x35D07F)
/**
 * Blue LED / info
 */
#define COLOR_BLUE lv_color_hex(0x38BDF8)
/**
 * Humidity / warning
 */
#define COLOR_AMBER lv_color_hex(0xF5A524)
/**
 * Neutral track behind sliders, arcs, ticks
 */
#define COLOR_TRACK lv_color_hex(0x2B3444)
/**
 * Opacity of an LED that is switched off
 */
#define OPA_DIM (255 * 22 / 100)


#ifndef LV_XML_EVAL_STRING_BUF_SIZE
    #define LV_XML_EVAL_STRING_BUF_SIZE 256
#endif

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL VARIABLES
 **********************/

/*-------------------
 * Permanent screens
 *------------------*/

/*----------------
 * Global styles
 *----------------*/

extern lv_style_t style_screen;
extern lv_style_t style_card;
extern lv_style_t style_inset;
extern lv_style_t style_section;
extern lv_style_t style_muted;
extern lv_style_t style_label_small;
extern lv_style_t style_small_body;
extern lv_style_t style_title;
extern lv_style_t style_value;
extern lv_style_t style_clock;
extern lv_style_t style_switch_red;
extern lv_style_t style_switch_green;
extern lv_style_t style_switch_blue;
extern lv_style_t style_scrollbar;

/*----------------
 * Fonts
 *----------------*/

/* Targets: any */
extern lv_font_t * font_symbols;
extern lv_font_t * font_small;
extern lv_font_t * font_body;
extern lv_font_t * font_label;
extern lv_font_t * font_title;
extern lv_font_t * font_value;
extern lv_font_t * font_clock;


/*----------------
 * Images
 *----------------*/

/* Targets: any */
extern const void * logo;

/*----------------
 * Subjects
 *----------------*/

extern lv_subject_t sensor_temp_c;
extern lv_subject_t sensor_temp_f;
extern lv_subject_t sensor_humidity;
extern lv_subject_t sensor_status;
extern lv_subject_t clock_hour_angle;
extern lv_subject_t clock_min_angle;
extern lv_subject_t clock_sec_angle;
extern lv_subject_t clock_time;
extern lv_subject_t clock_date;
extern lv_subject_t player_x;
extern lv_subject_t player_y;
extern lv_subject_t player_name;
extern lv_subject_t joy_x;
extern lv_subject_t joy_y;
extern lv_subject_t net_ip;
extern lv_subject_t net_peer;
extern lv_subject_t net_connected;
extern lv_subject_t led_red;
extern lv_subject_t led_green;
extern lv_subject_t led_blue;
extern lv_subject_t chat_log;
extern lv_subject_t cmd_led_red;
extern lv_subject_t cmd_led_green;
extern lv_subject_t cmd_led_blue;
extern lv_subject_t cmd_sensor_refresh;
extern lv_subject_t cmd_chat_message;
extern lv_subject_t cmd_chat_send;
extern lv_subject_t show_keyboard;
extern lv_subject_t camera_image;
extern lv_subject_t camera_image_blur;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/*----------------
 * Event Callbacks
 *----------------*/

void ui_chat_send(lv_event_t * e);

/**
 * Initialize the component library
 */

void desktop_ui_init_gen(const char * asset_path);

/**********************
 *      MACROS
 **********************/

/**********************
 *   POST INCLUDES
 **********************/

/*Include all the widgets, components and screens of this library*/
#include "components/ui_button_gen.h"
#include "components/ui_chart_gen.h"
#include "components/ui_chat_gen.h"
#include "components/ui_clock_gen.h"
#include "components/ui_game_gen.h"
#include "components/ui_header_gen.h"
#include "components/ui_joystick_gen.h"
#include "components/ui_led_gen.h"
#include "components/ui_leds_gen.h"
#include "components/ui_panel_gen.h"
#include "components/ui_row_gen.h"
#include "components/ui_sensors_gen.h"
#include "components/ui_stat_gen.h"
#include "components/ui_toggle_gen.h"
#include "screens/main_screen_gen.h"

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*DESKTOP_UI_GEN_H*/