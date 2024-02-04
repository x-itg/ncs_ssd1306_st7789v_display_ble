#include <zephyr/device.h>
#include <zephyr/drivers/display.h>
#include <lvgl.h>
#include <stdio.h>
#include <string.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#define DISPLAY_DRIVER "ST7789V"

#define LOG_LEVEL CONFIG_LOG_DEFAULT_LEVEL
LOG_MODULE_REGISTER(app);

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
	{65, 40},
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
void one_btn_event_handler(lv_event_t *e)
{
	lv_obj_t *two = lv_event_get_user_data(e);

	// 加载屏幕TWO,动画效果为LV_SCR_LOAD_ANIM_FADE_ON,切换时间为100ms,延迟0ms后从第一屏开始切换，切换完成后隐藏屏幕二
	lv_scr_load_anim(two, LV_SCR_LOAD_ANIM_FADE_OUT, 100, 0, false);
}
void two_btn_event_handler(lv_event_t *e)
{
	lv_obj_t *one = lv_event_get_user_data(e);

	// 加载屏幕one,动画效果为LV_SCR_LOAD_ANIM_FADE_ON,切换时间为100ms,延迟0ms后从第二屏开始切换，切换完成后隐藏屏幕一
	lv_scr_load_anim(one, LV_SCR_LOAD_ANIM_FADE_OUT, 100, 0, false);
}

void main(void)
{
	// 显示设备
	const struct device *display_dev;

	display_dev = device_get_binding(DISPLAY_DRIVER);
	if (!device_is_ready(display_dev))
	{
		LOG_ERR("Device not ready, aborting test");
		return;
	}

	display_blanking_off(display_dev);

	// 输入设备
	lv_indev_drv_init(&indev_drv);
	indev_drv.type = LV_INDEV_TYPE_BUTTON;
	indev_drv.read_cb = button_read;
	indev_button = lv_indev_drv_register(&indev_drv);
	lv_indev_set_button_points(indev_button, btn_points); // 将按键与坐标连接

	// 输入设备组
	lv_group_t *g = lv_group_create();
	lv_group_set_default(g); //    lv_group_add_obj(g,label);
	lv_indev_set_group(indev_button, g);

	// 创建页面1
	lv_obj_t *one = lv_obj_create(lv_scr_act());							  // 在默认屏上创建obj对象
	lv_obj_set_style_bg_color(one, lv_color_hex(0xeeffcc), LV_STATE_DEFAULT); // obj背景色设成黄色
	lv_obj_set_size(one, 130, 130);											  // 设置到屏幕大小

	// 创建页面2
	lv_obj_t *two = lv_obj_create(NULL);									  // 创建新屏幕但未加载到显示
	lv_obj_set_style_bg_color(two, lv_color_hex(0x00d8db), LV_STATE_DEFAULT); // 背影色设成蓝色
	lv_obj_set_size(two, 130, 130);											  // 设置到屏幕大小

	// 从one创建按钮one_btn=
	lv_obj_t *one_btn = lv_btn_create(one);
	lv_obj_align(one_btn, LV_ALIGN_TOP_MID, 0, 20);
	lv_obj_t *label = lv_label_create(one_btn); // 创建label
	lv_obj_align(label, LV_ALIGN_CENTER, 0, 0); // label居屏幕中心对齐
	lv_label_set_text(label, "ONE");			// label显示ONE
	lv_obj_add_event_cb(one_btn, one_btn_event_handler, LV_EVENT_CLICKED, two);

	// 从two创建按钮two_btn ，注册回调函数，
	lv_obj_t *two_btn = lv_btn_create(two);
	lv_obj_align(two_btn, LV_ALIGN_TOP_MID, 0, 20);
	label = lv_label_create(two_btn);			// 创建label
	lv_obj_align(label, LV_ALIGN_CENTER, 0, 0); // 居中对齐
	lv_label_set_text(label, "TWO");			// label上显示TWO
	lv_obj_add_event_cb(two_btn, two_btn_event_handler, LV_EVENT_CLICKED, one);
 
	lv_label_get_text(label);
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
