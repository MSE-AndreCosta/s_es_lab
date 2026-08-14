#include <stdio.h>
#include <pthread.h>

#include "led/led.h"
#include "joystick/joystick.h"
#include "sht/sht.h"
#include "msgq/msgq.h"
#include "server/server.h"
#include "receiver/receiver.h"
#include "broadcast/broadcast.h"
#include "transmitter/transmitter.h"
#include "reader/reader.h"
#include "game/game.h"
#include "oled/oled.h"

int main(void)
{
	pthread_t broadcast_thread;
	pthread_t tx_thread;
	pthread_t rx_thread;
	pthread_t reader_thread;
	pthread_t *threads[] = {&broadcast_thread, &tx_thread, &rx_thread, &reader_thread};
	/* every task loops on `shutdown`, so it must not start out as stack garbage */
	server_t server = {0};

	for (size_t i = 0; i < sizeof(server.leds) / sizeof(server.leds[0]); ++i) {
		server.leds[i].id = (led_id_t)i;
	}
	game_init(&server.player_position);

	msgq_init(&server.tx);
	led_init();
	joystick_init();
	sht_init();
	oled_init();
	oled_display_ip();

	pthread_mutex_init(&server.data_mutex, NULL);
	pthread_create(&broadcast_thread, NULL, broadcast_sender_task, &server);
	pthread_create(&tx_thread, NULL, transmitter_task, &server);
	pthread_create(&rx_thread, NULL, receiver_task, &server);
	pthread_create(&reader_thread, NULL, reader_task, &server);
	for (size_t i = 0; i < sizeof(threads) / sizeof(threads[0]); ++i) {
		pthread_join(*threads[i], NULL);
	}
	led_deinit();
	joystick_deinit();
	sht_deinit();
	oled_deinit();

	return 0;
}
