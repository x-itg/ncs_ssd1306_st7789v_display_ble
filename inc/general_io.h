#ifndef __GENERAL_IO_H
#define __GENERAL_IO_H
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <soc.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/types.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/sys/__assert.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/device.h>
struct led
{
    struct gpio_dt_spec spec; // gpio设备的专有
    uint8_t num;
};

struct printk_data_t
{
    void *fifo_reserved; /* 1st word reserved for use by fifo */
    uint32_t led;
    uint32_t cnt;
};
// #define LED0_NODE DT_ALIAS(led0)
// #define LED1_NODE DT_ALIAS(led1)
// #define LED2_NODE DT_ALIAS(led2)
// #define LED3_NODE DT_ALIAS(led3)
#define LED4_NODE DT_ALIAS(led4)
#define LED5_NODE DT_ALIAS(led5)
#define KEY0_NODE DT_ALIAS(sw0) // 别名

extern struct k_fifo printk_fifo;

#endif