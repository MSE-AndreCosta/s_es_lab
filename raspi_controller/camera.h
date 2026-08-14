#ifndef CAMERA_H
#define CAMERA_H

#include <stdbool.h>

/** Stream used when neither @ref camera_init nor @c $CAMERA_URL provide one. */
#define CAMERA_DEFAULT_URL "http://10.229.248.38:8000/stream"

/**
 * @brief Starts acquiring frames from an MJPEG stream.
 *
 * A worker thread pulls the stream with libcurl and cuts it into JPEG frames.
 * The frames are published to the @c camera_image subject by an LVGL timer, so
 * the subject is only ever written from the LVGL thread.
 *
 * Must be called after LVGL and the UI are initialized.
 *
 * @param url  Stream URL, or NULL to use @c $CAMERA_URL / @ref CAMERA_DEFAULT_URL.
 * @return true on success, false if the acquisition could not be started.
 */
bool camera_init(const char *url);

/**
 * @brief Stops the acquisition and releases the frame buffers.
 */
void camera_deinit(void);

#endif /*CAMERA_H*/
