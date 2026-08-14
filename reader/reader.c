#include "protocol/protocol.h"
#include "reader.h"
#include "led/led.h"
#include "joystick/joystick.h"
#include <sys/epoll.h>
#include <sys/timerfd.h>
#include <unistd.h>
#include <pthread.h>
#include "server/server.h"
#include "joystick/joystick.h"
#include "led/led.h"
#include "sht/sht.h"
#include <stdlib.h>

#define SENSOR_POLL_INTERVAL_MS 500

static void update_joystick(server_t *server)
{
	joystick_t dir;
	if (joystick_read_direction(&dir) <= 0) {
		return;
	}
	if (dir == server->joystick) {
		return;
	}

	pthread_mutex_lock(&server->data_mutex);
	server->joystick = dir;
	pthread_mutex_unlock(&server->data_mutex);

	protocol_message_t *message = malloc(sizeof(*message));
	if (!message) {
		return;
	}

	message->type = PMT_JOYSTICK;
	message->data.joystick = dir;
	msgq_push(&server->tx, message);
}
static void poll_sensor(server_t *server, int timer_fd)
{
	uint64_t exp;
	read(timer_fd, &exp, sizeof(exp));

	sensor_data_t sensor;
	if (sht_read(&sensor) != 0) {
		return;
	}

	if (sensor.temp_c == server->sensor_data.temp_c &&
	    sensor.temp_f == server->sensor_data.temp_f &&
	    sensor.humidity == server->sensor_data.humidity) {
		return;
	}

	pthread_mutex_lock(&server->data_mutex);
	server->sensor_data = sensor;
	pthread_mutex_unlock(&server->data_mutex);

	protocol_message_t *message = malloc(sizeof(*message));
	if (!message) {
		return;
	}
	message->type = PMT_SENSOR;
	message->data.sensor = sensor;
	msgq_push(&server->tx, message);
}
static void poll_leds(server_t *server)

{
	led_t leds[3];
	led_id_t ids[3] = {LED_RED, LED_GREEN, LED_BLUE};
	for (size_t i = 0; i < sizeof(leds) / sizeof(leds[0]); i++) {
		bool state;
		if (led_get(ids[i], &state) != 0) {
			return;
		}
		leds[i].id = ids[i];
		leds[i].on = state;
	}

	bool equal = false;

	for (size_t i = 0; i < 3; i++) {
		if (leds[i].on == server->leds[i].on) {
			continue;
		}
		pthread_mutex_lock(&server->data_mutex);
		server->leds[i].on = leds[i].on;
		pthread_mutex_unlock(&server->data_mutex);

		protocol_message_t *message = malloc(sizeof(*message));
		if (!message) {
			return;
		}
		message->type = PMT_LED;
		message->data.led = leds[i];
		msgq_push(&server->tx, message);
	}
}

void *reader_task(void *arg)
{
	server_t *server = (server_t *)arg;

	int gpio_fd = joystick_get_fd();

	int timer_fd = timerfd_create(CLOCK_MONOTONIC, 0);
	struct itimerspec its = {
		.it_value = {.tv_sec = 0, .tv_nsec = SENSOR_POLL_INTERVAL_MS * 1000000L},
		.it_interval = {.tv_sec = 0, .tv_nsec = SENSOR_POLL_INTERVAL_MS * 1000000L}};
	timerfd_settime(timer_fd, 0, &its, NULL);

	int epfd = epoll_create1(0);
	struct epoll_event ev = {0};

	ev.events = EPOLLIN;
	ev.data.fd = gpio_fd;
	epoll_ctl(epfd, EPOLL_CTL_ADD, gpio_fd, &ev);

	ev.events = EPOLLIN;
	ev.data.fd = timer_fd;
	epoll_ctl(epfd, EPOLL_CTL_ADD, timer_fd, &ev);

	struct epoll_event events[2];

	while (!server->shutdown) {
		int n = epoll_wait(epfd, events, 2, 1000);
		if (n < 0) {
			continue;
		}

		for (int i = 0; i < n; i++) {
			if (events[i].data.fd == gpio_fd) {
				update_joystick(server);
			} else if (events[i].data.fd == timer_fd) {
				poll_sensor(server, timer_fd);
				poll_leds(server);
			}
		}
	}
	close(timer_fd);
	return NULL;
}
