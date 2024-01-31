#include "general_io.h"
#include "display.h"
K_FIFO_DEFINE(printk_fifo);
static struct gpio_dt_spec keyled = GPIO_DT_SPEC_GET_OR(LED2_NODE, gpios, {0});
static struct gpio_dt_spec button = GPIO_DT_SPEC_GET_OR(KEY0_NODE, gpios, {0});
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
        // printk("Toggled led%d; counter=%d\n",
        //        rx_data->led, rx_data->cnt);
        k_free(rx_data);
    }
}

int keyread(void)
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

// 消息接收并打印任务，消息发出一定要及时消化掉 不然会死机
K_THREAD_DEFINE(uart_out_id, 1024, uart_out, NULL, NULL, NULL, 7, 0, 0);

// 读取按键任务
K_THREAD_DEFINE(keyread_id, 1024, keyread, NULL, NULL, NULL, 7, 0, 0);
