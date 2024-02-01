# һ��ncs oled ble ssd1306

һ������BLE��ssd1306 ��ʾ��һ��������

## ����

app.overlay ���Ѿ�������������
prj.confʹ����Ӧ��������ʹ��ѡ��

| nrf52dk_nrf52832 | SSD1306 |
| ---------------- | ------- |
| P0.26            | SDA     |
| P0.27            | SCL     |

# ����ncs_st7789v_lvgl_nrf52832_helloword

һ���� [LVGL](https://lvgl.io/) ���ı���ǩ�ļ������� use a `ST7789 - 130x130 TFT` display

## ����

app.overlay ���Ѿ�������������
prj.confʹ����Ӧ��������ʹ��ѡ��

| nrf52dk_nrf52832 | ST7789V |
| ---------------- | ------- |
| P0.21            | RESET   |
| P0.25            | BL      |
| P0.27            | CS      |
| P0.26            | D/C     |
| P0.29            | SCLK    |
| P0.28            | MOSI    |

# ����ncs_st7789v_lvgl-nrf52832

һ���� [LVGL](https://lvgl.io/) ��widget�ļ������� use a `ST7789 - 130x130 TFT` display

## ����

app.overlay ���Ѿ�������������
prj.confʹ����Ӧ��������ʹ��ѡ��

| nrf52dk_nrf52832 | ST7789V |
| ---------------- | ------- |
| P0.21            | RESET   |
| P0.25            | BL      |
| P0.27            | CS      |
| P0.26            | D/C     |
| P0.29            | SCLK    |
| P0.28            | MOSI    |

# �ġ�ncs_st7789v_nrf52832

һ��use a `ST7789 - 130x130 TFT` display ɫ��ļ�����

## ����

app.overlay ���Ѿ�������������
prj.confʹ����Ӧ��������ʹ��ѡ��

| nrf52dk_nrf52832 | ST7789V |
| ---------------- | ------- |
| P0.21            | RESET   |
| P0.25            | BL      |
| P0.27            | CS      |
| P0.26            | D/C     |
| P0.29            | SCLK    |
| P0.28            | MOSI    |


## Ӳ��

[Ӳ���õ���鿪����NRF52832DKӲ������ֲ� ](http://doc.iotxx.com/NRF52832DK%E7%A1%AC%E4%BB%B6%E8%AE%BE%E8%AE%A1%E6%89%8B%E5%86%8C) ���˹�����Nordic����NCS��̰��µ�һ�������ۺ�demo�����˰��� �� OLED ����ȣ���ΪNCS��ѧϰ�ʼǡ�

������̵�Ŀ���ǰ�sdk�����zephyr����nrf�����samples���������еĴ���������һ��ʵ��һ���ۺϵ�demo��

## �����

��������SDK ��NCS�±�̺�ESP IDF��࣬���һ�����ܾ��������̰Ѷ�����̺ϲ���һ����

* ʹ��nRF connect for Desktop�µ�ToolChain Manager���ع�������ʹ��VSCODE��Nordic�������SDK
* ����������githubʱ��
* ssh -N -D 127.0.0.1:8080 ubuntu@172.16.1.215    �������IP��
* git config --global https.proxy "socks5://127.0.0.1:8080"  #��ssh��ϵ�ʱ��socks5����
* git config --global https.proxy "socks5://127.0.0.1:8080"  #��ssh��ϵ�ʱ��socks5����
* ```ȡ��
  git config --global --unset http.proxy #���git����
  git config --global --unset https.proxy #���git����
  git config --list #�г�����
  ```
* sdk�ļ�����git clone nrf sdk���ļ�����Ϊnrf
* sdk�ļ���������west update���������ֿ�
* west update���ɹ�ʱ���԰�github�ϵĲֿ⵼�뵽gitee�ϣ�git clone gitee�ϵĲֿ⵽���أ��л�����Ҫ�İ汾��֧�󣬿�����sdk��ӦĿ¼��ȥ���ٴ�west update��Ӧ���ܳɹ��ˡ�
* ���л�toolchain��sdk�汾����Ҫ�������build
* VSCODE Nordic����е��ToolChainѡ��Open Terminal Profile��nRF Connect�ն�����west build�Ǳ��� ����west flash�����أ�Ҳ����ֱ�ӵ��VSCODE Nordic�����ACTIONS�����build��Flash��ť���в�����


