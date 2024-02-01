/*
 *  main.c - Application main entry point
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
#include "display.h"
#include <zephyr/logging/log.h>
#include <zephyr/device.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/__assert.h>
LOG_MODULE_REGISTER(main, 3);
/* size of stack area used by each thread */
#define STACKSIZE 1024

/* scheduling priority used by each thread */
#define PRIORITY 7

#define LED0_NODE DT_ALIAS(led0)
#define LED1_NODE DT_ALIAS(led1)
#define LED2_NODE DT_ALIAS(led2)
#define LED3_NODE DT_ALIAS(led3)
#define LED4_NODE DT_ALIAS(led4)
#define LED5_NODE DT_ALIAS(led5)

#define SW0_NODE DT_ALIAS(sw0) // 别名

#if !DT_NODE_HAS_STATUS(LED0_NODE, okay)
#error "Unsupported board: led0 devicetree alias is not defined"
#endif

#if !DT_NODE_HAS_STATUS(LED1_NODE, okay)
#error "Unsupported board: led1 devicetree alias is not defined"
#endif

struct printk_data_t
{
    void *fifo_reserved; /* 1st word reserved for use by fifo */
    uint32_t led;
    uint32_t cnt;
};

K_FIFO_DEFINE(printk_fifo);

struct led
{
    struct gpio_dt_spec spec; // gpio设备的专有
    uint8_t num;
};

static const struct gpio_dt_spec keyled0 = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const struct gpio_dt_spec keyled1 = GPIO_DT_SPEC_GET(LED1_NODE, gpios);
static const struct gpio_dt_spec keyled2 = GPIO_DT_SPEC_GET(LED2_NODE, gpios);
static const struct gpio_dt_spec keyled3 = GPIO_DT_SPEC_GET(LED3_NODE, gpios);
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(SW0_NODE, gpios);

static struct gpio_callback button_cb_data;
void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    printk("Button pressed at %" PRIu32 "\n", k_cycle_get_32());
}

void uart_out(void)
{

    while (1)
    {
        struct printk_data_t *rx_data = k_fifo_get(&printk_fifo,
                                                   K_FOREVER);
        printk("Toggled led%d; counter=%d\n",
               rx_data->led, rx_data->cnt);
        k_free(rx_data);
    }
}

int keyread(void)
{
    int ret;
    // check dt
    if (!gpio_is_ready_dt(&keyled0) || !gpio_is_ready_dt(&keyled1) || !gpio_is_ready_dt(&keyled2) || !gpio_is_ready_dt(&keyled3) || !gpio_is_ready_dt(&button))
    {
        return 0;
    }

    // button
    ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
    ret = gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_TO_ACTIVE);
    gpio_init_callback(&button_cb_data, button_pressed, BIT(button.pin));
    gpio_add_callback(button.port, &button_cb_data);

    // led

    ret = gpio_pin_configure_dt(&keyled0, GPIO_OUTPUT);
    ret = gpio_pin_configure_dt(&keyled1, GPIO_OUTPUT);
    ret = gpio_pin_configure_dt(&keyled2, GPIO_OUTPUT);
    ret = gpio_pin_configure_dt(&keyled3, GPIO_OUTPUT);

    while (1)
    {
        /* If we have an LED, match its state to the button's. */
        int val = gpio_pin_get_dt(&button);

        if (val >= 0)
        {
            gpio_pin_set_dt(&keyled0, val);
            gpio_pin_set_dt(&keyled1, val);
            gpio_pin_set_dt(&keyled2, val);
            gpio_pin_set_dt(&keyled3, val);
        }
        k_msleep(1);
    }

    return 0;
}
// main任务 天然有的任务 不用K_THREAD_DEFINE创建
int main(void)
{
    LOG_INF("%s", __func__);

    display_init();

    display_play();
}

// 消息接收并打印任务，消息发出一定要及时消化掉 不然会死机
K_THREAD_DEFINE(uart_out_id, STACKSIZE, uart_out, NULL, NULL, NULL,
                PRIORITY, 0, 0);

K_THREAD_DEFINE(keyread_id, STACKSIZE, keyread, NULL, NULL, NULL,
                PRIORITY, 0, 0);
