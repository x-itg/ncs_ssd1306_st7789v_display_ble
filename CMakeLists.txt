#
# Copyright (c) 2018 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#
cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(NONE)

# NORDIC SDK APP START
target_sources(app PRIVATE src/main.c)
target_sources(app PRIVATE display.c)
target_sources(app PRIVATE mainthread.c)
target_sources(app PRIVATE fonts/font5x7.c)
target_sources(app PRIVATE fonts/font8x8.c)
target_sources(app PRIVATE fonts/font10x16.c)
target_sources(app PRIVATE fonts/font15x24.c)
target_sources(app PRIVATE fonts/font20x32.c)

target_include_directories(app PRIVATE ./)

# Include UART ASYNC API adapter
target_sources_ifdef(CONFIG_BT_NUS_UART_ASYNC_ADAPTER app PRIVATE
  src/uart_async_adapter.c
)

# NORDIC SDK APP END

zephyr_library_include_directories(.)
