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

// 物理按键
#include <zephyr/drivers/gpio.h>

#define SW0_NODE DT_ALIAS(sw0) // 别名
#define SW1_NODE DT_ALIAS(sw1) // 别名
#define SW2_NODE DT_ALIAS(sw2) // 别名
#define SW3_NODE DT_ALIAS(sw3) // 别名
static const struct gpio_dt_spec button0 = GPIO_DT_SPEC_GET(SW0_NODE, gpios);
static const struct gpio_dt_spec button1 = GPIO_DT_SPEC_GET(SW1_NODE, gpios);
static const struct gpio_dt_spec button2 = GPIO_DT_SPEC_GET(SW2_NODE, gpios);
static const struct gpio_dt_spec button3 = GPIO_DT_SPEC_GET(SW3_NODE, gpios);
int keyfour[4] = {0, 0, 0, 0};
char KEY_VAL = -1;
lv_indev_drv_t indev_drv; // 物理按键
lv_indev_t *indev_button;
lv_point_t btn_points[4] = {
	{30, 98},
	{110, 98},
	{0, 0},
	{0, 0},
};

static void button_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
	static uint8_t last_btn = 0;
	/*Get the pressed button's ID*/
	int8_t btn_act;
	btn_act = KEY_VAL; // 将实际键值传递给响应键值
	KEY_VAL = -1;
	if (btn_act >= 0)
	{
		data->state = LV_INDEV_STATE_PR;
		last_btn = btn_act;
	}
	else
	{
		data->state = LV_INDEV_STATE_REL;
	}
	/*Save the last pressed button's ID*/
	data->btn_id = last_btn;
}

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
	display_blanking_off(display_dev);

	lv_indev_drv_init(&indev_drv);
	indev_drv.type = LV_INDEV_TYPE_BUTTON;
	indev_drv.read_cb = button_read;
	indev_button = lv_indev_drv_register(&indev_drv);
	lv_indev_set_button_points(indev_button, btn_points); // 将按键与坐标连接

	// lv_group_t *g = lv_group_create();
	// lv_group_set_default(g); //    lv_group_add_obj(g,label);
	// lv_indev_set_group(indev_button, g);

	setup_ui(&guider_ui);
	events_init(&guider_ui);

	while (1)
	{
		lv_task_handler();
		k_sleep(K_MSEC(10));
	}
}
int keyread(void)
{
	int ret;
	// check dt
	if (!gpio_is_ready_dt(&button0) || !gpio_is_ready_dt(&button1) || !gpio_is_ready_dt(&button2) || !gpio_is_ready_dt(&button3))
	{
		return 0;
	}

	gpio_pin_configure_dt(&button0, GPIO_INPUT);
	gpio_pin_configure_dt(&button1, GPIO_INPUT);
	gpio_pin_configure_dt(&button2, GPIO_INPUT);
	gpio_pin_configure_dt(&button3, GPIO_INPUT);

	while (1)
	{
		keyfour[0] = gpio_pin_get_dt(&button0);
		keyfour[1] = gpio_pin_get_dt(&button1);
		keyfour[2] = gpio_pin_get_dt(&button2);
		keyfour[3] = gpio_pin_get_dt(&button3);
		if (keyfour[0] == 1)
		{
			KEY_VAL = 0;
		}
		if (keyfour[1] == 1)
		{
			KEY_VAL = 1;
		}
		if (keyfour[2] == 1)
		{
			KEY_VAL = 2;
		}
		if (keyfour[3] == 1)
		{
			KEY_VAL = 3;
		}
		k_msleep(10);
	}

	return 0;
}

K_THREAD_DEFINE(keyread_id, 1024, keyread, NULL, NULL, NULL, 7, 0, 0);
