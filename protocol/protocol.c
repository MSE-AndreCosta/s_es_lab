
#include "protocol.h"
#include <cJSON.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <assert.h>

#define FIELD_TAG  "tag"
#define FIELD_DATA "data"

#define LED                 "led"
#define JOYSTICK            "joystick"
#define TAG_SENSOR          "sensor"
#define TAG_REQUEST         "request"
#define TAG_PLAYER_POSITION "player_position"
#define IP_ADDRESS          "ip_address"
#define MESSAGE             "message"
#define ID                  "id"
#define ON                  "on"
#define TEMP_C              "celsius"
#define TEMP_F              "fahrenheit"
#define HUMIDITY            "humidity"
#define POS_X               "x"
#define POS_Y               "y"

static const char *response_tag_to_string(protocol_message_type_t type);
static bool response_tag_from_string(const char *str, protocol_message_type_t *out);
static cJSON *encode_data(const protocol_message_t *message);

bool protocol_message_decode(const char *buffer, size_t len, protocol_message_t *out_message)
{
	if (!buffer || len == 0 || !out_message) {
		return false;
	}

	char *json_str = strndup((const char *)buffer, len);
	if (!json_str) {
		return false;
	}
	cJSON *root = cJSON_Parse(json_str);
	free(json_str);
	if (!root) {
		return false;
	}

	const cJSON *tag = cJSON_GetObjectItemCaseSensitive(root, FIELD_TAG);
	if (!cJSON_IsString(tag) || !tag->valuestring) {
		return false;
	}

	if (response_tag_from_string(tag->valuestring, &out_message->type) != 0) {
		return false;
	}

	const cJSON *data = cJSON_GetObjectItemCaseSensitive(root, FIELD_DATA);
	if (!cJSON_IsObject(data)) {
		return false;
	}

	switch (out_message->type) {
	case PMT_JOYSTICK: {
		const cJSON *joystick = cJSON_GetObjectItemCaseSensitive(data, JOYSTICK);
		if (!cJSON_IsNumber(joystick)) {
			return false;
		}
		out_message->data.joystick = joystick->valueint;
		break;
	}

	case PMT_SENSOR: {
		const cJSON *temp_c = cJSON_GetObjectItemCaseSensitive(data, TEMP_C);
		if (!cJSON_IsNumber(temp_c)) {
			return false;
		}
		const cJSON *temp_f = cJSON_GetObjectItemCaseSensitive(data, TEMP_F);
		if (!cJSON_IsNumber(temp_f)) {
			return false;
		}
		const cJSON *humidity = cJSON_GetObjectItemCaseSensitive(data, HUMIDITY);
		if (!cJSON_IsNumber(humidity)) {
			return false;
		}
		out_message->data.sensor.temp_c = temp_c->valueint;
		out_message->data.sensor.temp_f = temp_f->valueint;
		out_message->data.sensor.humidity = humidity->valueint;
		break;
	}
	case PMT_REQUEST:
		break;
	case PMT_PLAYER_POSITION: {
		const cJSON *x = cJSON_GetObjectItemCaseSensitive(data, POS_X);
		if (!cJSON_IsNumber(x)) {
			return false;
		}
		const cJSON *y = cJSON_GetObjectItemCaseSensitive(data, POS_Y);
		if (!cJSON_IsNumber(y)) {
			return false;
		}
		out_message->data.player_position.x = x->valueint;
		out_message->data.player_position.y = y->valueint;
		break;
	}
	case PMT_IP_ADDRESS: {
		const cJSON *ip_address = cJSON_GetObjectItemCaseSensitive(data, IP_ADDRESS);
		if (!cJSON_IsString(ip_address)) {
			return false;
		}
		strncpy((char *)out_message->data.ipv4, ip_address->valuestring,
			sizeof(out_message->data.ipv4) - 1);
		out_message->data.ipv4[sizeof(out_message->data.ipv4) - 1] = '\0';
		break;
	}
	case PMT_LED: {
		const cJSON *id = cJSON_GetObjectItemCaseSensitive(data, ID);
		if (!cJSON_IsNumber(id)) {
			return false;
		}
		const cJSON *on = cJSON_GetObjectItemCaseSensitive(data, ON);
		if (!cJSON_IsNumber(on)) {
			return false;
		}
		out_message->data.led.id = id->valueint;
		out_message->data.led.on = on->valueint;
		break;
	}
	case PMT_CHAT_MESSAGE: {
		const cJSON *id = cJSON_GetObjectItemCaseSensitive(data, ID);
		if (!cJSON_IsNumber(id)) {
			return false;
		}
		const cJSON *data = cJSON_GetObjectItemCaseSensitive(data, MESSAGE);
		if (!cJSON_IsString(data)) {
			return false;
		}
		strncpy((char *)out_message->data.message.data, data->valuestring,
			sizeof(out_message->data.message.data) - 1);

		out_message->data.message.data[sizeof(out_message->data.message) - 1] = '\0';
		out_message->data.message.id = id->valueint;
		break;
	}
	}

	cJSON_Delete(root);
	return true;

err:
	cJSON_Delete(root);
	return NULL;
}

char *protocol_message_encode(const protocol_message_t *message)
{
	if (!message) {
		return false;
	}
	const char *tag_str = response_tag_to_string(message->type);
	if (!tag_str) {
		return false;
	}

	cJSON *root = cJSON_CreateObject();
	if (!root) {
		return false;
	}

	if (!cJSON_AddStringToObject(root, FIELD_TAG, tag_str)) {
		goto err;
	}

	cJSON *data = encode_data(message);
	if (!data) {
		goto err;
	}
	cJSON_AddItemToObject(root, FIELD_DATA, data);

	char *json_str = cJSON_PrintUnformatted(root);
	cJSON_Delete(root);

	if (!json_str) {
		return NULL;
	}

	return json_str;
err:
	cJSON_Delete(root);
	return NULL;
}

void protocol_message_delete(char *payload)
{
	cJSON_free(payload);
}

static const char *response_tag_to_string(protocol_message_type_t type)
{
	switch (type) {
	case PMT_JOYSTICK:
		return JOYSTICK;
	case PMT_SENSOR:
		return TAG_SENSOR;
	case PMT_REQUEST:
		return TAG_REQUEST;
	case PMT_PLAYER_POSITION:
		return TAG_PLAYER_POSITION;
	case PMT_IP_ADDRESS:
		return IP_ADDRESS;
	case PMT_LED:
		return LED;
	case PMT_CHAT_MESSAGE:
		return MESSAGE;
	}
	return NULL;
}

static bool response_tag_from_string(const char *str, protocol_message_type_t *out)
{
	assert(str);
	assert(out);
	if (strcmp(str, JOYSTICK) == 0) {
		*out = PMT_JOYSTICK;
	} else if (strcmp(str, TAG_SENSOR) == 0) {
		*out = PMT_SENSOR;
	} else if (strcmp(str, TAG_REQUEST) == 0) {
		*out = PMT_REQUEST;
	} else if (strcmp(str, TAG_PLAYER_POSITION) == 0) {
		*out = PMT_PLAYER_POSITION;
	} else if (strcmp(str, IP_ADDRESS) == 0) {
		*out = PMT_IP_ADDRESS;
	} else if (strcmp(str, LED) == 0) {
		*out = PMT_LED;
	} else {
		return false;
	}
	return 0;
}

static cJSON *encode_data(const protocol_message_t *message)
{
	cJSON *data = cJSON_CreateObject();
	if (!data) {
		return NULL;
	}
	switch (message->type) {
	case PMT_JOYSTICK:
		if (!cJSON_AddNumberToObject(data, JOYSTICK, message->data.joystick)) {
			goto err;
		}
		break;
	case PMT_SENSOR: {
		if (!cJSON_AddNumberToObject(data, TEMP_C, message->data.sensor.temp_c)) {
			goto err;
		}
		if (!cJSON_AddNumberToObject(data, TEMP_F, message->data.sensor.temp_f)) {
			goto err;
		}
		if (!cJSON_AddNumberToObject(data, HUMIDITY, message->data.sensor.humidity)) {
			goto err;
		}
		break;
	}
	case PMT_REQUEST:
		break;
	case PMT_PLAYER_POSITION:
		if (!cJSON_AddNumberToObject(data, POS_X, message->data.player_position.x)) {
			goto err;
		}
		if (!cJSON_AddNumberToObject(data, POS_Y, message->data.player_position.y)) {
			goto err;
		}
		break;
	case PMT_IP_ADDRESS: {
		if (!cJSON_AddStringToObject(data, IP_ADDRESS, message->data.ipv4)) {
			goto err;
		}
		break;
	}
	case PMT_LED:
		if (!cJSON_AddNumberToObject(data, ID, message->data.led.id)) {
			goto err;
		}
		if (!cJSON_AddNumberToObject(data, ON, message->data.led.on)) {
			goto err;
		}
		break;
	case PMT_CHAT_MESSAGE:
		if (!cJSON_AddNumberToObject(data, ID, message->data.message.id)) {
			goto err;
		}
		if (!cJSON_AddStringToObject(data, MESSAGE, message->data.message.data)) {
			goto err;
		}
		break;
	}
	return data;
err:

	cJSON_Delete(data);
	return NULL;
}
