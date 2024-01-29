#include "general_io.h"

static const struct led led4 = {
    .spec = GPIO_DT_SPEC_GET_OR(LED4_NODE, gpios, {0}),
    .num = 4,
};

static const struct led led5 = {
    .spec = GPIO_DT_SPEC_GET_OR(LED5_NODE, gpios, {0}),
    .num = 5,
};

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

// 灯0闪 任务 闪后发出消息
K_THREAD_DEFINE(blink0_id, 1024, blink0, NULL, NULL, NULL, 7, 0, 0);

// 灯1闪 任务 闪后发出消息
K_THREAD_DEFINE(blink1_id, 1024, blink1, NULL, NULL, NULL, 7, 0, 0);