# nrf52dk_nrf52832 lvgl demo ST7789V

A simple example of how to use a [LVGL](https://lvgl.io/) widget in 

board and a `ST7789 - 130x130 TFT` display

# ����

app.overlay ���Ѿ�������������
prj.confʹ����Ӧ��������ʹ��ѡ��
| nrf52dk_nrf52832 | ST7789V    |
| ---------------- | ---------- |
| P0.21            | RESET      |
| P0.25            | BL         |
| P0.27            | CS         |
| P0.26            | D/C        |
| P0.29            | SCLK       |
| P0.28            | MOSI       |

# ����

```
$ west build #�� ��vscode ncs��� ACTIONS�����build��ť
```

# ����

```
$ west flash #�� ��vscode ncs��� ACTIONS�����FLASH��ť
```


