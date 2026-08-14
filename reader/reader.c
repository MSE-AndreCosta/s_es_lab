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

#define SENSOR_POLL_INTERVAL_MS 500

void *reader_task(void *arg)
{
    server_t *server = (server_t *)arg;

    int gpio_fd = joystick_get_fd();

    int timer_fd = timerfd_create(CLOCK_MONOTONIC, 0);
    struct itimerspec its = {
        .it_value    = {.tv_sec = 0, .tv_nsec = SENSOR_POLL_INTERVAL_MS * 1000000L},
        .it_interval = {.tv_sec = 0, .tv_nsec = SENSOR_POLL_INTERVAL_MS * 1000000L}
    };
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
        if (n < 0) continue;

        for (int i = 0; i < n; i++) {
            if (events[i].data.fd == gpio_fd) {
                joystick_t dir;
                if (joystick_read_direction(&dir) > 0) {
                    pthread_mutex_lock(&server->data_mutex);
                    server->joystick = dir;
                    pthread_mutex_unlock(&server->data_mutex);
                }
            } else if (events[i].data.fd == timer_fd) {
                uint64_t exp;
                read(timer_fd, &exp, sizeof(exp));

                sensor_data_t sensor;
                if (sht_read(&sensor) == 0) {
                pthread_mutex_lock(&server->data_mutex);
                server->sensor_data = sensor;
                pthread_mutex_unlock(&server->data_mutex);
            }


                led_t leds[3];
                led_id_t ids[3] = {LED_RED, LED_GREEN, LED_BLUE};
                for (int j = 0; j < 3; j++) {
                    bool state;
                    if (led_get(ids[j], &state) == 0) {
                        leds[j].id = ids[j];
                        leds[j].on = state;
                    }
                }

                pthread_mutex_lock(&server->data_mutex);
                server->sensor_data = sensor;
                for (int j = 0; j < 3; j++) {
                    server->leds[j] = leds[j];
                }
                pthread_mutex_unlock(&server->data_mutex);
            }
        }
    }

    close(timer_fd);
    return NULL;
}


