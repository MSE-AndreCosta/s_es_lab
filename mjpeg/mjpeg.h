#ifndef MJPEG_H
#define MJPEG_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @brief Called for every complete JPEG frame found in the stream.
 *
 * @param data       Frame bytes, starting at SOI and ending after EOI. Only
 *                   valid for the duration of the call.
 * @param len        Length of @p data.
 * @param user_data  Opaque pointer given to @ref mjpeg_parser_init.
 */
typedef void (*mjpeg_frame_cb_t)(const uint8_t *data, size_t len, void *user_data);

/**
 * @brief Splits a `multipart/x-mixed-replace` byte stream into JPEG frames.
 *
 * The multipart boundaries and part headers are simply skipped: the frames are
 * delimited by the JPEG markers themselves, which also makes the parser work
 * for endpoints that return a bare sequence of JPEG images.
 */
typedef struct {
	uint8_t *data;
	size_t len;
	size_t capacity;
	size_t max_frame_size;
	mjpeg_frame_cb_t on_frame;
	void *user_data;
} mjpeg_parser_t;

/**
 * @brief Initializes a parser.
 *
 * @param parser          Parser to initialize.
 * @param max_frame_size  Above this many buffered bytes without a complete
 *                        frame the buffer is dropped, so a non-JPEG stream
 *                        cannot make the parser grow forever.
 * @param on_frame        Called for every complete frame, must not be NULL.
 * @param user_data       Passed back to @p on_frame.
 */
void mjpeg_parser_init(mjpeg_parser_t *parser, size_t max_frame_size, mjpeg_frame_cb_t on_frame,
		       void *user_data);

/**
 * @brief Feeds freshly received bytes to the parser.
 *
 * @param parser  Parser to feed.
 * @param data    Received bytes.
 * @param len     Number of bytes in @p data.
 *
 * @return false when the bytes could not be buffered, true otherwise.
 */
bool mjpeg_parser_feed(mjpeg_parser_t *parser, const void *data, size_t len);

/**
 * @brief Drops the buffered bytes, to be called when the stream restarts.
 */
void mjpeg_parser_reset(mjpeg_parser_t *parser);

/**
 * @brief Releases the parser buffer.
 */
void mjpeg_parser_deinit(mjpeg_parser_t *parser);

#endif /*MJPEG_H*/
