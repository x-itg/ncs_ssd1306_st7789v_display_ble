# 一、ncs oled ble ssd1306

一个集合BLE和ssd1306 显示的一个简单例程

## 配置

app.overlay 中已经调整引脚配置
prj.conf使能相应驱动编译使能选项

| nrf52dk_nrf52832 | SSD1306 |
| ---------------- | ------- |
| P0.26            | SDA     |
| P0.27            | SCL     |

# 二、ncs_st7789v_lvgl_nrf52832_helloword

一个用 [LVGL](https://lvgl.io/) 的文本标签的简单例程序 use a `ST7789 - 130x130 TFT` display

## 配置

app.overlay 中已经调整引脚配置
prj.conf使能相应驱动编译使能选项

| nrf52dk_nrf52832 | ST7789V |
| ---------------- | ------- |
| P0.21            | RESET   |
| P0.25            | BL      |
| P0.27            | CS      |
| P0.26            | D/C     |
| P0.29            | SCLK    |
| P0.28            | MOSI    |

# 三、ncs_st7789v_lvgl-nrf52832

一个用 [LVGL](https://lvgl.io/) 的widget的简单例程序 use a `ST7789 - 130x130 TFT` display

## 配置

app.overlay 中已经调整引脚配置
prj.conf使能相应驱动编译使能选项

| nrf52dk_nrf52832 | ST7789V |
| ---------------- | ------- |
| P0.21            | RESET   |
| P0.25            | BL      |
| P0.27            | CS      |
| P0.26            | D/C     |
| P0.29            | SCLK    |
| P0.28            | MOSI    |

# 四、ncs_st7789v_nrf52832

一个use a `ST7789 - 130x130 TFT` display 色块的简单例程

## 配置

app.overlay 中已经调整引脚配置
prj.conf使能相应驱动编译使能选项

| nrf52dk_nrf52832 | ST7789V |
| ---------------- | ------- |
| P0.21            | RESET   |
| P0.25            | BL      |
| P0.27            | CS      |
| P0.26            | D/C     |
| P0.29            | SCLK    |
| P0.28            | MOSI    |


## 硬件

[硬件用的这块开发板NRF52832DK硬件设计手册 ](http://doc.iotxx.com/NRF52832DK%E7%A1%AC%E4%BB%B6%E8%AE%BE%E8%AE%A1%E6%89%8B%E5%86%8C) ，此工程是Nordic最新NCS编程包下的一个外设综合demo集合了按键 灯 OLED 任务等，作为NCS的学习笔记。

这个工程的目的是把sdk下面的zephyr或者nrf下面的samples各个例程中的代码整合在一起实现一个综合的demo。

## 环境搭建

工具链和SDK （NCS下编程和ESP IDF差不多，完成一定功能就是找例程把多个例程合并成一个）

* 使用nRF connect for Desktop下的ToolChain Manager下载工具链，使用VSCODE的Nordic插件下载SDK
* 以下连不上github时：
* ssh -N -D 127.0.0.1:8080 ubuntu@172.16.1.215    （外面的IP）
* git config --global https.proxy "socks5://127.0.0.1:8080"  #和ssh配合的时候socks5代理
* git config --global https.proxy "socks5://127.0.0.1:8080"  #和ssh配合的时候socks5代理
* ```取消
  git config --global --unset http.proxy #清除git代理
  git config --global --unset https.proxy #清除git代理
  git config --list #列出配置
  ```
* sdk文件夹中git clone nrf sdk，文件名改为nrf
* sdk文件夹下运行west update导入其他仓库
* west update不成功时可以把github上的仓库导入到gitee上，git clone gitee上的仓库到本地，切换到想要的版本分支后，拷贝到sdk对应目录中去。再次west update就应该能成功了。
* 当切换toolchain和sdk版本后需要重新添加build
* VSCODE Nordic插件中点击ToolChain选择Open Terminal Profile打开nRF Connect终端输入west build是编译 输入west flash是下载，也可以直接点击VSCODE Nordic插件中ACTIONS下面的build和Flash按钮进行操作。


