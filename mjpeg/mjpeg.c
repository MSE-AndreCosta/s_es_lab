#include "mjpeg.h"

#include <stdlib.h>
#include <string.h>

#define JPEG_SOI_0 0xFF
#define JPEG_SOI_1 0xD8
#define JPEG_EOI_0 0xFF
#define JPEG_EOI_1 0xD9

static bool reserve(mjpeg_parser_t *parser, size_t size)
{
	if (parser->capacity >= size) {
		return true;
	}
	size_t capacity = parser->capacity ? parser->capacity : 65536;
	while (capacity < size) {
		capacity *= 2;
	}
	uint8_t *data = realloc(parser->data, capacity);
	if (!data) {
		return false;
	}
	parser->data = data;
	parser->capacity = capacity;
	return true;
}

/**
 * @brief Drops the first @p count buffered bytes.
 */
static void consume(mjpeg_parser_t *parser, size_t count)
{
	if (count >= parser->len) {
		parser->len = 0;
		return;
	}
	memmove(parser->data, parser->data + count, parser->len - count);
	parser->len -= count;
}

/**
 * @brief Finds the two byte marker @p b0 @p b1, starting the search at @p from.
 * @return Index of the marker, or -1 when it is not in the buffer.
 */
static ssize_t find_marker(const uint8_t *data, size_t len, size_t from, uint8_t b0, uint8_t b1)
{
	for (size_t i = from; i + 1 < len; ++i) {
		if (data[i] == b0 && data[i + 1] == b1) {
			return (ssize_t)i;
		}
	}
	return -1;
}

void mjpeg_parser_init(mjpeg_parser_t *parser, size_t max_frame_size, mjpeg_frame_cb_t on_frame,
		       void *user_data)
{
	memset(parser, 0, sizeof(*parser));
	parser->max_frame_size = max_frame_size;
	parser->on_frame = on_frame;
	parser->user_data = user_data;
}

bool mjpeg_parser_feed(mjpeg_parser_t *parser, const void *data, size_t len)
{
	if (!parser || !data) {
		return false;
	}
	if (!reserve(parser, parser->len + len)) {
		return false;
	}
	memcpy(parser->data + parser->len, data, len);
	parser->len += len;

	for (;;) {
		ssize_t soi = find_marker(parser->data, parser->len, 0, JPEG_SOI_0, JPEG_SOI_1);
		if (soi < 0) {
			/* keep the last byte, a marker can be split across two reads */
			consume(parser, parser->len > 1 ? parser->len - 1 : 0);
			return true;
		}
		consume(parser, (size_t)soi);

		ssize_t eoi = find_marker(parser->data, parser->len, 2, JPEG_EOI_0, JPEG_EOI_1);
		if (eoi < 0) {
			if (parser->max_frame_size && parser->len > parser->max_frame_size) {
				parser->len = 0;
			}
			return true;
		}

		const size_t frame_len = (size_t)eoi + 2;
		if (parser->on_frame) {
			parser->on_frame(parser->data, frame_len, parser->user_data);
		}
		consume(parser, frame_len);
	}
}

void mjpeg_parser_reset(mjpeg_parser_t *parser)
{
	if (parser) {
		parser->len = 0;
	}
}

void mjpeg_parser_deinit(mjpeg_parser_t *parser)
{
	if (!parser) {
		return;
	}
	free(parser->data);
	parser->data = NULL;
	parser->len = 0;
	parser->capacity = 0;
}
