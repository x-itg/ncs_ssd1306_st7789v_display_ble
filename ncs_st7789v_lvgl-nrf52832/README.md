# nrf52dk_nrf52832 lvgl demo ST7789V

A simple example of how to use a [LVGL](https://lvgl.io/) widget in 

board and a `ST7789 - 130x130 TFT` display

# 配置

app.overlay 中已经调整引脚配置
prj.conf使能相应驱动编译使能选项
| nrf52dk_nrf52832 | ST7789V    |
| ---------------- | ---------- |
| P0.21            | RESET      |
| P0.25            | BL         |
| P0.27            | CS         |
| P0.26            | D/C        |
| P0.29            | SCLK       |
| P0.28            | MOSI       |

# 编译

```
$ west build #或 用vscode ncs插件 ACTIONS下面的build按钮
```

# 烧入

```
$ west flash #或 用vscode ncs插件 ACTIONS下面的FLASH按钮
```


