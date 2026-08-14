#include <assert.h>
#include <lvgl.h>
#include <desktop_ui.h>

#include "camera.h"
#include "controller.h"
#include "msgq/msgq.h"
#include "ui_interface.h"
#include "receiver.h"
static controller_t controller;

static void display_delete_cb(lv_event_t *e)
{
	controller.shutdown = true;
	msgq_push(&controller.ui_rx, NULL);
	msgq_push(&controller.ui_tx, NULL);
}

int main(void)
{
	pthread_t receiver_thread;

	msgq_init(&controller.ui_tx);
	msgq_init(&controller.ui_rx);

	if (!ui_interface_init(&controller.ui_tx, &controller.ui_rx)) {
		return 1;
	}

	if (!receiver_init(&controller)) {
		return 1;
	}

	if (!camera_init(NULL)) {
		LV_LOG_WARN("the camera stream is not available");
	}

	pthread_create(&receiver_thread, NULL, receiver_task, &controller);

	ui_interface_loop(&controller);
	pthread_join(receiver_thread, NULL);
	camera_deinit();

	return 0;
}
