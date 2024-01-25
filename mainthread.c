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

static const struct led led0 = {
    .spec = GPIO_DT_SPEC_GET_OR(LED0_NODE, gpios, {0}),
    .num = 0,
};

static const struct led led1 = {
    .spec = GPIO_DT_SPEC_GET_OR(LED1_NODE, gpios, {0}),
    .num = 1,
};
static const struct led led2 = {
    .spec = GPIO_DT_SPEC_GET_OR(LED2_NODE, gpios, {0}),
    .num = 2,
};

static const struct led led3 = {
    .spec = GPIO_DT_SPEC_GET_OR(LED3_NODE, gpios, {0}),
    .num = 3,
};

static const struct led led4 = {
    .spec = GPIO_DT_SPEC_GET_OR(LED4_NODE, gpios, {0}),
    .num = 4,
};

static const struct led led5 = {
    .spec = GPIO_DT_SPEC_GET_OR(LED5_NODE, gpios, {0}),
    .num = 5,
};
static struct gpio_dt_spec keyled = GPIO_DT_SPEC_GET_OR(DT_ALIAS(led2), gpios, {0});
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET_OR(SW0_NODE, gpios, {0});
static struct gpio_callback button_cb_data;
void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    printk("Button pressed at %" PRIu32 "\n", k_cycle_get_32());
}
void blink(const struct led *led, uint32_t sleep_ms, uint32_t id)
{
    const struct gpio_dt_spec *spec = &led->spec;
    int cnt = 0;
    int ret;

    if (!device_is_ready(spec->port))
    {
        printk("Error: %s device is not ready\n", spec->port->name);
        return;
    }

    ret = gpio_pin_configure_dt(spec, GPIO_OUTPUT);
    if (ret != 0)
    {
        printk("Error %d: failed to configure pin %d (LED '%d')\n",
               ret, spec->pin, led->num);
        return;
    }

    while (1)
    {
        gpio_pin_set(spec->port, spec->pin, cnt % 2);

        struct printk_data_t tx_data = {.led = id, .cnt = cnt};

        size_t size = sizeof(struct printk_data_t);
        char *mem_ptr = k_malloc(size);
        __ASSERT_NO_MSG(mem_ptr != 0);

        memcpy(mem_ptr, &tx_data, size);

        k_fifo_put(&printk_fifo, mem_ptr);

        k_msleep(sleep_ms);
        cnt++;
    }
}

void blink0(void)
{
    blink(&led4, 100, 0);
}

void blink1(void)
{
    blink(&led5, 300, 1);
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

void keyread(void)
{
    int ret;

    if (!gpio_is_ready_dt(&button))
    {
        printk("Error: button device %s is not ready\n",
               button.port->name);
        return 0;
    }

    ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
    if (ret != 0)
    {
        printk("Error %d: failed to configure %s pin %d\n",
               ret, button.port->name, button.pin);
        return 0;
    }

    ret = gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_TO_ACTIVE);

    if (ret != 0)
    {
        printk("Error %d: failed to configure interrupt on %s pin %d\n",
               ret, button.port->name, button.pin);
        return 0;
    }

    gpio_init_callback(&button_cb_data, button_pressed, BIT(button.pin));
    gpio_add_callback(button.port, &button_cb_data);
    printk("Set up button at %s pin %d\n", button.port->name, button.pin);

    if (keyled.port && !gpio_is_ready_dt(&keyled))
    {
        printk("Error %d: LED device %s is not ready; ignoring it\n",
               ret, keyled.port->name);
        keyled.port = NULL;
    }
    if (keyled.port)
    {
        ret = gpio_pin_configure_dt(&keyled, GPIO_OUTPUT);
        if (ret != 0)
        {
            printk("Error %d: failed to configure LED device %s pin %d\n",
                   ret, keyled.port->name, keyled.pin);
            keyled.port = NULL;
        }
        else
        {
            printk("Set up LED at %s pin %d\n", keyled.port->name, keyled.pin);
        }
    }

    printk("Press the button\n");
    if (keyled.port)
    {
        while (1)
        {
            /* If we have an LED, match its state to the button's. */
            int val = gpio_pin_get_dt(&button);

            if (val >= 0)
            {
                gpio_pin_set_dt(&keyled, val);
            }
            k_msleep(1);
        }
    }
    return 0;
}
// 灯0闪 任务 闪后发出消息
K_THREAD_DEFINE(blink0_id, STACKSIZE, blink0, NULL, NULL, NULL,
                PRIORITY, 0, 0);

// 灯1闪 任务 闪后发出消息
K_THREAD_DEFINE(blink1_id, STACKSIZE, blink1, NULL, NULL, NULL,
                PRIORITY, 0, 0);

// 消息接收并打印任务，消息发出一定要及时消化掉 不然会死机
K_THREAD_DEFINE(uart_out_id, STACKSIZE, uart_out, NULL, NULL, NULL,
                PRIORITY, 0, 0);

K_THREAD_DEFINE(keyread_id, STACKSIZE, keyread, NULL, NULL, NULL,
                PRIORITY, 0, 0);
