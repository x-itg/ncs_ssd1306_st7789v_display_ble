### 基本情况

[硬件用的这块开发板NRF52832DK硬件设计手册 ](http://doc.iotxx.com/NRF52832DK%E7%A1%AC%E4%BB%B6%E8%AE%BE%E8%AE%A1%E6%89%8B%E5%86%8C) ，此工程是Nordic最新NCS编程包下的一个外设综合demo集合了按键 灯 OLED 任务等，作为NCS的学习笔记。

这个工程的目的是把sdk下面的zephyr或者nrf下面的samples各个例程中的代码整合在一起实现一个综合的demo。

### 工具链和SDK

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

# ssd1306_zephyr

## Overview 已经被我修改成ncs v2.4.1运行

This project implements a SSD1306 OLED display connected to a nRF52 PCA10040 (nRF52832) board running Zephyr version 2.1.
In theory, this project could be ported to other Zephyr-supported boards, but this has not (yet) been tried.

Also this project shows how to include custom fonts.

## Hardware

See photo in *docs* directory for view of hardware configuration.
Below is a list hardware components.

* Nordic PCA10040 Eval Board (nRF52832)
* Adafruit 128x32 I2C OLED display (SSD1306)
* Bread board
* Ribbon cable with male headers.

Wire connections as follows

* P0.27 <--> SCL
* P0.26 <--> SDA
* P0.02 <--> RST  (optional)
* VDD   <--> Vin
* GND   <--> GND

## Software

This project was built with Zephyr 2.1.99 and selects the Nordic PCA10040 board ().
Change the "`set(BOARD nrf52_pca10040)`" in the CMakeFile.txt for other supported boards.

The following componets need to be configured though "make menuconfig".

* Drivers->i2c: I2C-master support for your board/SOC.
* Drivers->display: Display + SSD1306
* Subsys->fb: FrameBuffer (cfb)
* General Kernel Options->Other Kernel Object Option: Heap Memory Pool Size

The tricky part of the software configuration seems to be in setting up the **.overlay* file and the *prj.conf* file.  This requires some knowledge about the build-configuration process for zephyr.
Hopefully, this project can be used as an example of how to navigate though this procedure.

The difficult parts are setting up the I2C driver for the target board. In this project the nRF52 I2C support is well-known and relatively straight-forward in configuring.  Other SOCs and boards are not always so easy.

**NOTE:** The Zephyr components work together to provide basic display support across a small number of display controllers. In order to provide a common feature set, many of the SSD1306's more advanced feartures are not supported. In other words, the display + frame-buffer support is more or less constrained to line-by-line console mode operations.  Read the header file *cfb.h* for API details.

Custom fonts are provided in this project and are held in the *fonts* directory. Currently there are five fonts: three fonts taken from the zephyr (*zephyr/subsys/fb/cfb_fonts*) and two custom fonts.

* font10x16  zephyr-copy
* font15x24  zephyr-copy
* font20x32  zephyr-copy
* font5x7 custom
* font8x8 custom

In display.c, there is the SELECTED_FONT_INDEX which selects the font index.

## Operation

The display.c program is rather simple, in that it runs a loop which writes out four different strings to the display: see the *display_play()* function.

## JLink RTT Output

可以在nordic vscode的插件中 Actions下面的nRF Kconfig GUI下面开启Use RTT console选项保存到工程，这样就能用jlink rtt viewer查看printk输出的调试信息了。If you have RTT enabled and the RTT client running, then you should see the following output.

```
    *** Booting Zephyr OS build zephyr-v2.1.0-581-g4e135d76a3c2  ***

    main_thread

    initialized SSD1306

    index[0] font width 10, font height 16

    index[1] font width 15, font height 24

    index[2] font width 20, font height 32

    index[3] font width 5, font height 8

    index[4] font width 8, font height 8

    selected font: index[4]

    x_res 128, y_res 32, ppt 8, rows 4, cols 128
```
