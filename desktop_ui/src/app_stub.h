/**
 * @file app_stub.h
 *
 * A STAND-IN for the application that owns the hardware and the link.
 *
 * This file is NOT part of the UI. It exists so the dashboard does something
 * when you run it before the real application is connected, and so the
 * boundary is visible: everything in here writes STATE subjects, and the UI
 * writes none of them.
 *
 * Replace it with your real application (or just stop calling
 * `app_stub_init()`) and the UI keeps working unchanged - it will simply show
 * whatever the new owner publishes.
 *
 * What it stands in for:
 *   - drives the LED outputs from cmd_led_*, then reports led_*
 *   - answers cmd_sensor_refresh with a reading  (VALUES ARE FAKE, see the .c)
 *   - transmits cmd_chat_message and echoes it into chat_log
 *   - feeds the clock subjects from the system time once a second
 */

#ifndef APP_STUB_H
#define APP_STUB_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Subscribe to the command subjects and start the clock. Call once, after the
 * UI library has been initialized.
 */
void app_stub_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*APP_STUB_H*/
