#include <zephyr/device.h>
#include <zephyr/drivers/display.h>
#include <lvgl.h>
#include <stdio.h>
#include <string.h>
#include <zephyr/kernel.h>
#include "lvgl_sample.h"
#include "gui_guider.h"
#include "events_init.h"
#include <zephyr/logging/log.h>
#define DISPLAY_DRIVER "ST7789V"

#define LOG_LEVEL CONFIG_LOG_DEFAULT_LEVEL
LOG_MODULE_REGISTER(app);
lv_ui guider_ui;
void main(void)
{
	const struct device *display_dev;

	display_dev = device_get_binding(DISPLAY_DRIVER);
	if (!device_is_ready(display_dev))
	{
		LOG_ERR("Device not ready, aborting test");
		return;
	}

	lv_sample();
	lv_task_handler();
	display_blanking_off(display_dev);

	setup_ui(&guider_ui);
	events_init(&guider_ui);
	while (1)
	{
		lv_task_handler();
		k_sleep(K_MSEC(10));
	}
}
