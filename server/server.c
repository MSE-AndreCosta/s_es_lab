#include <stdio.h>
#include <pthread.h>

#include "led/led.h"
#include "joystick/joystick.h"
#include "msgq/msgq.h"
#include "server/server.h"
#include "receiver/receiver.h"
#include "broadcast/broadcast.h"
#include "transmitter/transmitter.h"

int main(void)
{
	pthread_t broadcast_thread;
	pthread_t tx_thread;
	pthread_t rx_thread;
	pthread_t *threads[] = {&broadcast_thread, &tx_thread, &rx_thread};
	server_t server;

	msgq_init(&server.tx);
	led_init();
	joystick_init();

	pthread_mutex_init(&server.data_mutex, NULL);
	pthread_create(&broadcast_thread, NULL, broadcast_sender_task, &server);
	pthread_create(&tx_thread, NULL, transmitter_task, &server);
	pthread_create(&rx_thread, NULL, receiver_task, &server);

	for (size_t i = 0; i < sizeof(threads) / sizeof(threads[0]); ++i) {
		pthread_join(*threads[i], NULL);
	}
	led_deinit();
	joystick_deinit();

	return 0;
}
