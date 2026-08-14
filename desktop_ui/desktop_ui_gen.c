/**
 * @file desktop_ui_gen.c
 */

/*********************
 *      INCLUDES
 *********************/

#include "desktop_ui_gen.h"

#if defined(LV_USE_XML) && LV_USE_XML
#endif /* LV_USE_XML */

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void check_font(lv_font_t ** font, const char * name);

/**********************
 *  STATIC VARIABLES
 **********************/

static uint32_t desktop_ui_target = DESKTOP_UI_TARGET_ALL;

/*----------------
 * Translations
 *----------------*/

/**********************
 *  GLOBAL VARIABLES
 **********************/

/*--------------------
 *  Permanent screens
 *-------------------*/

/*----------------
 * Fonts
 *----------------*/

lv_font_t * font_symbols;
extern lv_font_t font_symbols_data;
lv_font_t * font_small;
extern lv_font_t font_small_data;
lv_font_t * font_body;
extern lv_font_t font_body_data;
lv_font_t * font_label;
extern lv_font_t font_label_data;
lv_font_t * font_title;
extern lv_font_t font_title_data;
lv_font_t * font_value;
extern lv_font_t font_value_data;
lv_font_t * font_clock;
extern lv_font_t font_clock_data;

/*----------------
 * Images
 *----------------*/

/* Targets: any */
const void * logo = NULL;
extern const void * logo_data;

/*----------------
 * Global styles
 *----------------*/

lv_style_t style_screen;
lv_style_t style_card;
lv_style_t style_inset;
lv_style_t style_section;
lv_style_t style_muted;
lv_style_t style_label_small;
lv_style_t style_small_body;
lv_style_t style_title;
lv_style_t style_value;
lv_style_t style_clock;
lv_style_t style_switch_red;
lv_style_t style_switch_green;
lv_style_t style_switch_blue;
lv_style_t style_scrollbar;

/*----------------
 * Subjects
 *----------------*/

lv_subject_t sensor_temp_c;
lv_subject_t sensor_temp_f;
lv_subject_t sensor_humidity;
lv_subject_t sensor_status;
lv_subject_t clock_hour_angle;
lv_subject_t clock_min_angle;
lv_subject_t clock_sec_angle;
lv_subject_t clock_time;
lv_subject_t clock_date;
lv_subject_t player_x;
lv_subject_t player_y;
lv_subject_t player_name;
lv_subject_t joy_x;
lv_subject_t joy_y;
lv_subject_t net_ip;
lv_subject_t net_peer;
lv_subject_t net_connected;
lv_subject_t led_red;
lv_subject_t led_green;
lv_subject_t led_blue;
lv_subject_t chat_log;
lv_subject_t cmd_led_red;
lv_subject_t cmd_led_green;
lv_subject_t cmd_led_blue;
lv_subject_t cmd_sensor_refresh;
lv_subject_t cmd_chat_message;
lv_subject_t cmd_chat_send;
lv_subject_t show_keyboard;
lv_subject_t camera_image;
lv_subject_t camera_image_blur;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void desktop_ui_init_gen(const char * asset_path)
{

    /*----------------
     * Fonts
     *----------------*/

    /* Targets: any */

    #if DESKTOP_UI_CHECK_COMPILE_TARGET(DESKTOP_UI_TARGET_ALL)
    if (desktop_ui_check_target(DESKTOP_UI_TARGET_ALL)) {
        if (!font_symbols) {
            /* font_symbols */
            /* get font 'font_symbols' from a C array */
            font_symbols = &font_symbols_data;

        }
        if (!font_small) {
            /* font_small */
            /* get font 'font_small' from a C array */
            font_small = &font_small_data;

        }
        if (!font_body) {
            /* font_body */
            /* get font 'font_body' from a C array */
            font_body = &font_body_data;

        }
        if (!font_label) {
            /* font_label */
            /* get font 'font_label' from a C array */
            font_label = &font_label_data;

        }
        if (!font_title) {
            /* font_title */
            /* get font 'font_title' from a C array */
            font_title = &font_title_data;

        }
        if (!font_value) {
            /* font_value */
            /* get font 'font_value' from a C array */
            font_value = &font_value_data;

        }
        if (!font_clock) {
            /* font_clock */
            /* get font 'font_clock' from a C array */
            font_clock = &font_clock_data;

        }
    }
    #endif

    /*----------------
     * Images
     *----------------*/

    /* Targets: any */
    #if DESKTOP_UI_CHECK_COMPILE_TARGET(DESKTOP_UI_TARGET_ALL)
    if (desktop_ui_check_target(DESKTOP_UI_TARGET_ALL)) {
        /* logo */
        if (!logo) {
            logo = &logo_data;
        }
    }
    #endif

    /*----------------
     * Global styles
     *----------------*/

    static bool style_inited = false;

    if (!style_inited) {
        /*Init all styles*/
        lv_style_init(&style_screen);
        lv_style_init(&style_card);
        lv_style_init(&style_inset);
        lv_style_init(&style_section);
        lv_style_init(&style_muted);
        lv_style_init(&style_label_small);
        lv_style_init(&style_small_body);
        lv_style_init(&style_title);
        lv_style_init(&style_value);
        lv_style_init(&style_clock);
        lv_style_init(&style_switch_red);
        lv_style_init(&style_switch_green);
        lv_style_init(&style_switch_blue);
        lv_style_init(&style_scrollbar);

        lv_style_set_bg_color(&style_screen, COLOR_BG);
        lv_style_set_bg_opa(&style_screen, (255 * 100 / 100));
        lv_style_set_text_color(&style_screen, COLOR_TEXT);
        lv_style_set_text_font(&style_screen, font_body);
        lv_style_set_bg_color(&style_card, COLOR_PANEL);
        lv_style_set_bg_opa(&style_card, (255 * 100 / 100));
        lv_style_set_border_color(&style_card, COLOR_BORDER);
        lv_style_set_border_width(&style_card, BORDER_WIDTH);
        lv_style_set_border_opa(&style_card, (255 * 100 / 100));
        lv_style_set_radius(&style_card, RADIUS_LG);
        lv_style_set_pad_all(&style_card, SPACE_LG);
        lv_style_set_bg_color(&style_inset, COLOR_INSET);
        lv_style_set_bg_opa(&style_inset, (255 * 100 / 100));
        lv_style_set_border_color(&style_inset, COLOR_BORDER);
        lv_style_set_border_width(&style_inset, BORDER_WIDTH);
        lv_style_set_radius(&style_inset, RADIUS_SM);
        lv_style_set_text_font(&style_section, font_label);
        lv_style_set_text_color(&style_section, COLOR_MUTED);
        lv_style_set_text_letter_space(&style_section, 1);
        lv_style_set_text_color(&style_muted, COLOR_MUTED);
        lv_style_set_text_font(&style_muted, font_small);
        lv_style_set_text_color(&style_label_small, COLOR_TEXT);
        lv_style_set_text_font(&style_label_small, font_label);
        lv_style_set_text_font(&style_small_body, font_small);
        lv_style_set_text_line_space(&style_small_body, 4);
        lv_style_set_text_color(&style_small_body, COLOR_TEXT);
        lv_style_set_text_font(&style_title, font_title);
        lv_style_set_text_color(&style_title, COLOR_TEXT);
        lv_style_set_text_font(&style_value, font_value);
        lv_style_set_text_color(&style_value, COLOR_TEXT);
        lv_style_set_text_font(&style_clock, font_clock);
        lv_style_set_text_letter_space(&style_clock, 1);
        lv_style_set_text_color(&style_clock, COLOR_TEXT);
        lv_style_set_bg_color(&style_switch_red, COLOR_RED);
        lv_style_set_bg_color(&style_switch_green, COLOR_GREEN);
        lv_style_set_bg_color(&style_switch_blue, COLOR_BLUE);
        lv_style_set_width(&style_scrollbar, SPACE_XS);
        lv_style_set_radius(&style_scrollbar, SPACE_XS);
        lv_style_set_bg_color(&style_scrollbar, COLOR_TRACK);
        lv_style_set_bg_opa(&style_scrollbar, (255 * 100 / 100));
        lv_style_set_pad_all(&style_scrollbar, 2);

        style_inited = true;
    }

    /*----------------
     * Subjects
     *----------------*/
    lv_subject_init_int(&sensor_temp_c, 23);
    lv_subject_set_min_value_int(&sensor_temp_c, -40);
    lv_subject_set_max_value_int(&sensor_temp_c, 125);
    lv_subject_init_int(&sensor_temp_f, 73);
    lv_subject_set_min_value_int(&sensor_temp_f, -40);
    lv_subject_set_max_value_int(&sensor_temp_f, 257);
    lv_subject_init_int(&sensor_humidity, 48);
    lv_subject_set_min_value_int(&sensor_humidity, 0);
    lv_subject_set_max_value_int(&sensor_humidity, 100);
    static char sensor_status_buf[UI_SUBJECT_STRING_LENGTH];
    static char sensor_status_prev_buf[UI_SUBJECT_STRING_LENGTH];
    lv_subject_init_string(&sensor_status,
                           sensor_status_buf,
                           sensor_status_prev_buf,
                           UI_SUBJECT_STRING_LENGTH,
                           "waiting for first reading"
                          );
    lv_subject_init_int(&clock_hour_angle, 1500);
    lv_subject_set_min_value_int(&clock_hour_angle, 0);
    lv_subject_set_max_value_int(&clock_hour_angle, 3600);
    lv_subject_init_int(&clock_min_angle, 2100);
    lv_subject_set_min_value_int(&clock_min_angle, 0);
    lv_subject_set_max_value_int(&clock_min_angle, 3600);
    lv_subject_init_int(&clock_sec_angle, 1800);
    lv_subject_set_min_value_int(&clock_sec_angle, 0);
    lv_subject_set_max_value_int(&clock_sec_angle, 3600);
    static char clock_time_buf[UI_SUBJECT_STRING_LENGTH];
    static char clock_time_prev_buf[UI_SUBJECT_STRING_LENGTH];
    lv_subject_init_string(&clock_time,
                           clock_time_buf,
                           clock_time_prev_buf,
                           UI_SUBJECT_STRING_LENGTH,
                           "05:35:30"
                          );
    static char clock_date_buf[UI_SUBJECT_STRING_LENGTH];
    static char clock_date_prev_buf[UI_SUBJECT_STRING_LENGTH];
    lv_subject_init_string(&clock_date,
                           clock_date_buf,
                           clock_date_prev_buf,
                           UI_SUBJECT_STRING_LENGTH,
                           "Wed 12 Aug 2026"
                          );
    lv_subject_init_int(&player_x, 244);
    lv_subject_set_min_value_int(&player_x, 0);
    lv_subject_set_max_value_int(&player_x, 488);
    lv_subject_init_int(&player_y, 96);
    lv_subject_set_min_value_int(&player_y, 0);
    lv_subject_set_max_value_int(&player_y, 192);
    static char player_name_buf[UI_SUBJECT_STRING_LENGTH];
    static char player_name_prev_buf[UI_SUBJECT_STRING_LENGTH];
    lv_subject_init_string(&player_name,
                           player_name_buf,
                           player_name_prev_buf,
                           UI_SUBJECT_STRING_LENGTH,
                           "P1"
                          );
    lv_subject_init_int(&joy_x, 0);
    lv_subject_set_min_value_int(&joy_x, -46);
    lv_subject_set_max_value_int(&joy_x, 46);
    lv_subject_init_int(&joy_y, 0);
    lv_subject_set_min_value_int(&joy_y, -46);
    lv_subject_set_max_value_int(&joy_y, 46);
    static char net_ip_buf[UI_SUBJECT_STRING_LENGTH];
    static char net_ip_prev_buf[UI_SUBJECT_STRING_LENGTH];
    lv_subject_init_string(&net_ip,
                           net_ip_buf,
                           net_ip_prev_buf,
                           UI_SUBJECT_STRING_LENGTH,
                           "192.168.1.42"
                          );
    static char net_peer_buf[UI_SUBJECT_STRING_LENGTH];
    static char net_peer_prev_buf[UI_SUBJECT_STRING_LENGTH];
    lv_subject_init_string(&net_peer,
                           net_peer_buf,
                           net_peer_prev_buf,
                           UI_SUBJECT_STRING_LENGTH,
                           "sensor-node-01"
                          );
    lv_subject_init_int(&net_connected, 1);
    lv_subject_set_min_value_int(&net_connected, 0);
    lv_subject_set_max_value_int(&net_connected, 1);
    lv_subject_init_int(&led_red, 0);
    lv_subject_set_min_value_int(&led_red, 0);
    lv_subject_set_max_value_int(&led_red, 1);
    lv_subject_init_int(&led_green, 0);
    lv_subject_set_min_value_int(&led_green, 0);
    lv_subject_set_max_value_int(&led_green, 1);
    lv_subject_init_int(&led_blue, 0);
    lv_subject_set_min_value_int(&led_blue, 0);
    lv_subject_set_max_value_int(&led_blue, 1);
    static char chat_log_buf[UI_SUBJECT_STRING_LENGTH];
    static char chat_log_prev_buf[UI_SUBJECT_STRING_LENGTH];
    lv_subject_init_string(&chat_log,
                           chat_log_buf,
                           chat_log_prev_buf,
                           UI_SUBJECT_STRING_LENGTH,
                           "[10:02] node-01: link established\n[10:02] you: hello there\n[10:03] node-01: temperature is stable"
                          );
    lv_subject_init_int(&cmd_led_red, 0);
    lv_subject_set_min_value_int(&cmd_led_red, 0);
    lv_subject_set_max_value_int(&cmd_led_red, 1);
    lv_subject_init_int(&cmd_led_green, 0);
    lv_subject_set_min_value_int(&cmd_led_green, 0);
    lv_subject_set_max_value_int(&cmd_led_green, 1);
    lv_subject_init_int(&cmd_led_blue, 0);
    lv_subject_set_min_value_int(&cmd_led_blue, 0);
    lv_subject_set_max_value_int(&cmd_led_blue, 1);
    lv_subject_init_int(&cmd_sensor_refresh, 0);
    lv_subject_set_min_value_int(&cmd_sensor_refresh, 0);
    lv_subject_set_max_value_int(&cmd_sensor_refresh, 65535);
    static char cmd_chat_message_buf[UI_SUBJECT_STRING_LENGTH];
    static char cmd_chat_message_prev_buf[UI_SUBJECT_STRING_LENGTH];
    lv_subject_init_string(&cmd_chat_message,
                           cmd_chat_message_buf,
                           cmd_chat_message_prev_buf,
                           UI_SUBJECT_STRING_LENGTH,
                           ""
                          );
    lv_subject_init_int(&cmd_chat_send, 0);
    lv_subject_set_min_value_int(&cmd_chat_send, 0);
    lv_subject_set_max_value_int(&cmd_chat_send, 65535);
    lv_subject_init_int(&show_keyboard, 0);
    lv_subject_set_min_value_int(&show_keyboard, 0);
    lv_subject_set_max_value_int(&show_keyboard, 1);
    lv_subject_init_pointer(&camera_image, logo);
    lv_subject_init_int(&camera_image_blur, 0);

    /*----------------
     * Translations
     *----------------*/

#if defined(LV_USE_XML) && LV_USE_XML
    /* Register widgets */

    /* Check all fonts / default if needed. This prevents fonts that are used in one target but
       defined in another from causing assertion failures during rendering of the Preview. */
    check_font(&font_symbols, "font_symbols");
    check_font(&font_small, "font_small");
    check_font(&font_body, "font_body");
    check_font(&font_label, "font_label");
    check_font(&font_title, "font_title");
    check_font(&font_value, "font_value");
    check_font(&font_clock, "font_clock");

    /* Register fonts */
    lv_xml_register_font(NULL, "font_symbols", font_symbols);
    lv_xml_register_font(NULL, "font_small", font_small);
    lv_xml_register_font(NULL, "font_body", font_body);
    lv_xml_register_font(NULL, "font_label", font_label);
    lv_xml_register_font(NULL, "font_title", font_title);
    lv_xml_register_font(NULL, "font_value", font_value);
    lv_xml_register_font(NULL, "font_clock", font_clock);

    /* Register subjects */
    lv_xml_register_subject(NULL, "sensor_temp_c", &sensor_temp_c);
    lv_xml_register_subject(NULL, "sensor_temp_f", &sensor_temp_f);
    lv_xml_register_subject(NULL, "sensor_humidity", &sensor_humidity);
    lv_xml_register_subject(NULL, "sensor_status", &sensor_status);
    lv_xml_register_subject(NULL, "clock_hour_angle", &clock_hour_angle);
    lv_xml_register_subject(NULL, "clock_min_angle", &clock_min_angle);
    lv_xml_register_subject(NULL, "clock_sec_angle", &clock_sec_angle);
    lv_xml_register_subject(NULL, "clock_time", &clock_time);
    lv_xml_register_subject(NULL, "clock_date", &clock_date);
    lv_xml_register_subject(NULL, "player_x", &player_x);
    lv_xml_register_subject(NULL, "player_y", &player_y);
    lv_xml_register_subject(NULL, "player_name", &player_name);
    lv_xml_register_subject(NULL, "joy_x", &joy_x);
    lv_xml_register_subject(NULL, "joy_y", &joy_y);
    lv_xml_register_subject(NULL, "net_ip", &net_ip);
    lv_xml_register_subject(NULL, "net_peer", &net_peer);
    lv_xml_register_subject(NULL, "net_connected", &net_connected);
    lv_xml_register_subject(NULL, "led_red", &led_red);
    lv_xml_register_subject(NULL, "led_green", &led_green);
    lv_xml_register_subject(NULL, "led_blue", &led_blue);
    lv_xml_register_subject(NULL, "chat_log", &chat_log);
    lv_xml_register_subject(NULL, "cmd_led_red", &cmd_led_red);
    lv_xml_register_subject(NULL, "cmd_led_green", &cmd_led_green);
    lv_xml_register_subject(NULL, "cmd_led_blue", &cmd_led_blue);
    lv_xml_register_subject(NULL, "cmd_sensor_refresh", &cmd_sensor_refresh);
    lv_xml_register_subject(NULL, "cmd_chat_message", &cmd_chat_message);
    lv_xml_register_subject(NULL, "cmd_chat_send", &cmd_chat_send);
    lv_xml_register_subject(NULL, "show_keyboard", &show_keyboard);
    lv_xml_register_subject(NULL, "camera_image", &camera_image);
    lv_xml_register_subject(NULL, "camera_image_blur", &camera_image_blur);

    /* Register callbacks */
    lv_xml_register_event_cb(NULL, "ui_chat_send", ui_chat_send);
#endif

    /* Register all the global assets so that they won't be created again when globals.xml is parsed.
     * While running in the editor skip this step to update the preview when the XML changes */
#if defined(LV_USE_XML) && LV_USE_XML && !defined(LV_EDITOR_PREVIEW)
    /* Register images */
    lv_xml_register_image(NULL, "logo", logo);
#endif

#if defined(LV_USE_XML) && LV_USE_XML == 0
    /*--------------------
     *  Permanent screens
     *-------------------*/
    /* If XML is enabled it's assumed that the permanent screens are created
     * manually from XML using lv_xml_create() */
#endif
}

void desktop_ui_set_target(uint32_t target)
{
    desktop_ui_target = target;
}

uint32_t desktop_ui_get_target(void)
{
    return desktop_ui_target;
}

bool desktop_ui_check_target(uint32_t target)
{
    return (desktop_ui_target & target) ? true : false;
}

/* Callbacks */
#if defined(LV_EDITOR_PREVIEW)
void __attribute__((weak)) ui_chat_send(lv_event_t * e)
{
    LV_UNUSED(e);
    LV_LOG("ui_chat_send was called\n");
}
#endif

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void check_font(lv_font_t ** font, const char * name)
{
    if (!(*font)) {
        *font = (lv_font_t *)LV_FONT_DEFAULT;
        LV_LOG_WARN("font `%s` was not set. Using `LV_FONT_DEFAULT` instead", name);
    }
}