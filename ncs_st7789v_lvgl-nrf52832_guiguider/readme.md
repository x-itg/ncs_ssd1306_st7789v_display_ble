# NXP Gui Guider

导出的generated和custom文件夹下面的所有.c .h全部复制到src文件夹下面


# 修改main.c
```
#include "gui_guider.h"
#include "events_init.h"

#define LOG_LEVEL CONFIG_LOG_DEFAULT_LEVEL

#define DISPLAY_DRIVER "ST7789V"

LOG_MODULE_REGISTER(app);

lv_ui guider_ui;

void main(void)
{
	const struct device *display_dev;

	display_dev = device_get_binding(DISPLAY_DRIVER);
 
	if (!device_is_ready(display_dev))
	{
		return;
	}

	setup_ui(&guider_ui);
 
	events_init(&guider_ui);
 
	while (1)
	{
		lv_task_handler();
   
		k_sleep(K_MSEC(10));
	}
}
```

# 修改pri.conf增加编译选项
```
CONFIG_LVGL=y
CONFIG_LV_MEM_CUSTOM=y
CONFIG_LV_USE_LOG=y
CONFIG_LV_COLOR_DEPTH_16=y
CONFIG_LV_COLOR_16_SWAP=y
CONFIG_LV_Z_BITS_PER_PIXEL=16
CONFIG_LV_USE_THEME_DEFAULT=y
CONFIG_LV_THEME_DEFAULT_DARK=y
CONFIG_LV_USE_METER=y
CONFIG_LV_Z_MEM_POOL_NUMBER_BLOCKS=8
CONFIG_LV_USE_QRCODE=y
CONFIG_LV_CONF_MINIMAL=y
CONFIG_LV_USE_LABEL=y
CONFIG_LV_LABEL_TEXT_SELECTION=y
CONFIG_LV_LABEL_LONG_TXT_HINT=y
CONFIG_LV_USE_CANVAS=y
CONFIG_LV_USE_BTN=y
```