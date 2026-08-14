#include "protocol/protocol.h"
#include "reader.h"
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <sys/timerfd.h>
#include <unistd.h>
#include <pthread.h>
#include "server/server.h"
#include "game/game.h"
#include "joystick/joystick.h"
#include "led/led.h"
#include "sht/sht.h"
#include <stdlib.h>

#define SENSOR_POLL_INTERVAL_MS 500
#define GAME_TICK_INTERVAL_MS   120

/** Resend the sensor values every N polls even when they did not change. */
#define SENSOR_HEARTBEAT_TICKS 10

/** Smallest change worth reporting, the sensor jitters below that. */
#define SENSOR_EPSILON 0.05

#define LED_COUNT 3

static void queue_message(server_t *server, const protocol_message_t *message)
{
	protocol_message_t *out_message = malloc(sizeof(*out_message));
	if (!out_message) {
		return;
	}
	*out_message = *message;
	msgq_push(&server->tx, out_message);
}

/**
 * @brief Publishes @p direction and moves the player accordingly.
 *
 * Called both from the edge event (so a press reacts immediately) and from the
 * game tick (so holding a direction keeps the player moving).
 */
static void apply_direction(server_t *server, joystick_t direction)
{
	protocol_message_t message;
	bool direction_changed;
	bool position_changed;
	vector2_t position;

	pthread_mutex_lock(&server->data_mutex);
	direction_changed = direction != server->joystick;
	server->joystick = direction;
	position_changed = game_move(&server->player_position, direction);
	position = server->player_position;
	pthread_mutex_unlock(&server->data_mutex);

	if (direction_changed) {
		message.type = PMT_JOYSTICK;
		message.data.joystick = direction;
		queue_message(server, &message);
	}

	if (position_changed) {
		message.type = PMT_PLAYER_POSITION;
		message.data.player_position = position;
		queue_message(server, &message);
	}
}

/**
 * @brief Drains the pending edge events and reacts to the new joystick state.
 *
 * The events *must* be consumed, otherwise the gpio descriptor stays readable
 * and epoll_wait() returns immediately forever.
 */
static void on_joystick_event(server_t *server)
{
	joystick_t edge_direction;
	joystick_t direction;

	if (joystick_read_direction(&edge_direction) < 0) {
		fprintf(stderr, "reader: failed to read the joystick edge events\n");
		return;
	}

	/* the line values are authoritative: an event batch may end on a release */
	if (joystick_poll_direction(&direction) != 0) {
		direction = edge_direction;
	}

	apply_direction(server, direction);
}

static void on_game_tick(server_t *server)
{
	joystick_t direction;

	if (joystick_poll_direction(&direction) != 0) {
		return;
	}
	apply_direction(server, direction);
}

static void poll_sensor(server_t *server, bool force)
{
	sensor_data_t sensor;
	if (sht_read(&sensor) != 0) {
		return;
	}

	if (!force && fabs(sensor.temp_c - server->sensor_data.temp_c) < SENSOR_EPSILON &&
	    fabs(sensor.temp_f - server->sensor_data.temp_f) < SENSOR_EPSILON &&
	    fabs(sensor.humidity - server->sensor_data.humidity) < SENSOR_EPSILON) {
		return;
	}

	pthread_mutex_lock(&server->data_mutex);
	server->sensor_data = sensor;
	pthread_mutex_unlock(&server->data_mutex);

	protocol_message_t message = {.type = PMT_SENSOR, .data.sensor = sensor};
	queue_message(server, &message);
}

static void poll_leds(server_t *server)
{
	const led_id_t ids[LED_COUNT] = {LED_RED, LED_GREEN, LED_BLUE};

	for (size_t i = 0; i < LED_COUNT; i++) {
		bool state;
		if (led_get(ids[i], &state) != 0) {
			return;
		}
		if (state == server->leds[i].on) {
			continue;
		}

		pthread_mutex_lock(&server->data_mutex);
		server->leds[i].id = ids[i];
		server->leds[i].on = state;
		pthread_mutex_unlock(&server->data_mutex);

		protocol_message_t message = {.type = PMT_LED, .data.led = {.id = ids[i], .on = state}};
		queue_message(server, &message);
	}
}

/**
 * @brief Creates a periodic timer descriptor firing every @p interval_ms.
 * @return The descriptor, or -1 on error.
 */
static int create_timer_fd(long interval_ms)
{
	int fd = timerfd_create(CLOCK_MONOTONIC, 0);
	if (fd < 0) {
		perror("reader: timerfd_create");
		return -1;
	}

	const struct timespec period = {
		.tv_sec = interval_ms / 1000,
		.tv_nsec = (interval_ms % 1000) * 1000000L,
	};
	const struct itimerspec its = {.it_value = period, .it_interval = period};

	if (timerfd_settime(fd, 0, &its, NULL) != 0) {
		perror("reader: timerfd_settime");
		close(fd);
		return -1;
	}
	return fd;
}

static int watch_fd(int epfd, int fd)
{
	struct epoll_event ev = {.events = EPOLLIN, .data.fd = fd};

	if (fd < 0) {
		return -1;
	}
	if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) != 0) {
		perror("reader: epoll_ctl");
		return -1;
	}
	return 0;
}

void *reader_task(void *arg)
{
	server_t *server = (server_t *)arg;
	struct epoll_event events[3];
	uint32_t sensor_ticks = 0;

	int gpio_fd = joystick_get_fd();
	if (gpio_fd < 0) {
		fprintf(stderr, "reader: the joystick is not initialized\n");
	}

	int sensor_fd = create_timer_fd(SENSOR_POLL_INTERVAL_MS);
	int game_fd = create_timer_fd(GAME_TICK_INTERVAL_MS);

	int epfd = epoll_create1(0);
	if (epfd < 0) {
		perror("reader: epoll_create1");
		goto out;
	}

	watch_fd(epfd, gpio_fd);
	if (watch_fd(epfd, sensor_fd) != 0) {
		goto out;
	}
	watch_fd(epfd, game_fd);

	while (!server->shutdown) {
		int n = epoll_wait(epfd, events, sizeof(events) / sizeof(events[0]), 1000);
		if (n < 0) {
			if (errno == EINTR) {
				continue;
			}
			perror("reader: epoll_wait");
			break;
		}

		for (int i = 0; i < n; i++) {
			uint64_t expirations;

			if (events[i].data.fd == gpio_fd) {
				on_joystick_event(server);
			} else if (events[i].data.fd == sensor_fd) {
				if (read(sensor_fd, &expirations, sizeof(expirations)) !=
				    (ssize_t)sizeof(expirations)) {
					perror("reader: sensor timer read");
					continue;
				}
				sensor_ticks++;
				poll_sensor(server, sensor_ticks % SENSOR_HEARTBEAT_TICKS == 0);
				poll_leds(server);
			} else if (events[i].data.fd == game_fd) {
				if (read(game_fd, &expirations, sizeof(expirations)) !=
				    (ssize_t)sizeof(expirations)) {
					perror("reader: game timer read");
					continue;
				}
				on_game_tick(server);
			}
		}
	}

out:
	if (epfd >= 0) {
		close(epfd);
	}
	if (sensor_fd >= 0) {
		close(sensor_fd);
	}
	if (game_fd >= 0) {
		close(game_fd);
	}
	return NULL;
}
