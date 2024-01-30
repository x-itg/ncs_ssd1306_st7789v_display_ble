/*
 * Copyright (c) 2018 PHYTEC Messtechnik GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <soc.h>

#include <zephyr/device.h>
#include <zephyr/display/cfb.h>
#include <stdio.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(display, 3);

#define DISPLAY_DRIVER "SSD1306"

static struct device *dev;
static uint16_t rows;
static uint8_t ppt;
static uint8_t font_width;
static uint8_t font_height;
// 0：font10x16 zephyr-copy
// 1：font15x24 zephyr-copy
// 2：font20x32 zephyr-copy
// 3：font5x7 custom
// 4：font8x8 custom
#define SELECTED_FONT_INDEX 0 // perhaps make this a config parameter

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
void display_play(void)
{
    uint8_t x_offset = 0;
    uint8_t y_offset;

    while (1)
    {

        for (int i = 0; i < rows; i++)
        {

            y_offset = i * 16; // ppt;

            switch (i)
            {
            case 0:
                cfb_print(dev, " fuck", x_offset, y_offset);
                break;
            case 1:
                cfb_print(dev, " good", x_offset, y_offset);
                break;
            case 2:
                cfb_print(dev, " better", x_offset, y_offset);
                break;
            case 3:
                cfb_print(dev, " best", x_offset, y_offset);
                break;
            default:
                break;
            }

            cfb_framebuffer_finalize(dev);

            k_sleep(K_MSEC(300));
        }

        cfb_framebuffer_clear(dev, false);

        if (x_offset > 50)
            x_offset = 0;
        else
            x_offset += 5;
    }
}

/*---------------------------------------------------------------------------*/
/*          20240130                                                         */
/*---------------------------------------------------------------------------*/
void display_init(void)
{
    dev = (struct device *)device_get_binding(DISPLAY_DRIVER);

    if (dev == NULL)
    {
        LOG_ERR("Device not found");
        return;
    }

    if (display_set_pixel_format(dev, PIXEL_FORMAT_MONO10) != 0) // 显示像素关联DISPLAY_DRIVER
    {
        LOG_ERR("Failed to set required pixel format");
        return;
    }

    LOG_INF("Binding to %s", DISPLAY_DRIVER);

    if (cfb_framebuffer_init(dev)) // 初始化Framebuffer
    {
        LOG_ERR("Framebuffer initialization failed!");
        return;
    }

    cfb_framebuffer_clear(dev, true); // 清屏

    display_blanking_off(dev);

    rows = cfb_get_display_parameter(dev, CFB_DISPLAY_ROWS);
    ppt = cfb_get_display_parameter(dev, CFB_DISPLAY_PPT);

    int num_fonts = cfb_get_numof_fonts(dev);

    for (int idx = 0; idx < num_fonts; idx++)
    {

        cfb_get_font_size(dev, idx, &font_width, &font_height); // 取得字体的宽度和高度

        LOG_INF("Index[%d] font dimensions %2dx%d",
                idx, font_width, font_height);
    }

    cfb_framebuffer_set_font(dev, SELECTED_FONT_INDEX);

    LOG_INF("Selected font: index[%d]", SELECTED_FONT_INDEX);

    cfb_framebuffer_invert(dev); // white on black

    LOG_INF("x_res %d, y_res %d, ppt %d, rows %d, cols %d",
            cfb_get_display_parameter(dev, CFB_DISPLAY_WIDTH),
            cfb_get_display_parameter(dev, CFB_DISPLAY_HEIGH),
            ppt,
            rows,
            cfb_get_display_parameter(dev, CFB_DISPLAY_COLS));
}