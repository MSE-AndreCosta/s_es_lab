#include <assert.h>
#include <stdio.h>
#include <lvgl.h>

int main(void)
{
	lv_init();
	lv_display_t *disp = lv_sdl_window_create(1280, 720);
	if (!disp) {
		perror("failed to initialize display");
		return 1;
	}

	lv_indev_t *kb = lv_sdl_keyboard_create();
	assert(kb);
	lv_group_t *g = lv_group_create();
	assert(g);

	lv_group_set_default(g);
	lv_indev_set_group(kb, g);
	lv_obj_t *image = lv_image_create(lv_screen_active());
	lv_obj_center(image);
	lv_image_set_src(image, "examples/img_lvgl_logo.jpg");

	while (1) {
		uint32_t ms = lv_timer_handler();
		ms = LV_MAX(ms, LV_DEF_REFR_PERIOD);
		lv_sleep_ms(ms);
	}
	return 0;
}
