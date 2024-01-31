# NCS DeviceTree

## 节点名

DeviceTree中的节点名称遵循以下命名规则：name@address。

## 节点属性 ​

DeviceTree中每个节点可以有几个属性来描述这个节点，属性是键值对。

属性：reg, #address-cells 与 #size-cells
reg属性代表此节点在总线上占用的地址和范围是由多对 (address, length)组合而成的#address-cells 和#sieze-cells则表示了这个总线上的节点的reg属性里，每个address和size要占用多少个uint32单元

https://www.cnblogs.com/jayant97/articles/17209392.html
https://blog.csdn.net/u011638175/article/details/121401579

## 节点号的获取NODEID

```
/dts-v1/;

/ {

    aliases {
        sensor-controller = &i2c1;/*别名*/
    };

    soc {
        i2c1: i2c@40002000 {
            compatible = "vnd,soc-i2c";/*实例号属性compatible属性描述的供应商和设备名*/
            label = "I2C_1";
            reg = <0x40002000 0x1000>;
            status = "okay";
            clock-frequency = < 100000 >;
        };
    };
};

//如何获取i2c@40002000节点了？
//注意：非字母数字的字符都需改为下划线，大写字母都需改为小写字母
DT_PATH(soc, i2c_40002000)//通过节点的全路径
DT_NODELABEL(i2c1)//通过节点标签
DT_ALIAS(sensor_controller)//通过alisa（别名）属性
DT_INST(x, vnd_soc_i2c)//实例号来获取  
```

## 节点属性的重写

- option1 &节点标签
- option2 &节点路径
- option3 以/aliase的方式添加虚拟节点
- option4 /chosen虚拟节点，为Zephyr Kernel选择特定设备（如日志串口）
- option5 管理引脚复用的/pinctrl节点
- option6 /zephyr,user
  
  ```
  //在<board>.dts有如下节点
  / {
        soc {
                serial0: serial@40002000 {
                        status = "okay";
                        current-speed = <115200>;
                        /* ... */
                };
        };
  };
  ```

//在.overlays文件中有2种方式重新配置属性
/* Option 1 */
&serial0 {
     current-speed = <9600>;
};

/* Option 2 */
&{/soc/serial@40002000} {
     current-speed = <9600>;
};

//例如<board>.dts中有&serial0节点，在overlay文件中单独添加aliase和chosen，方便使用
/ {
     aliases {
             my-serial = &serial0;
     };
};

/ {
     chosen {
             zephyr,console = &serial0;
     };
};

```
## 添加子节点
```

//例如<board>.dts中有&spi1节点，下面就可以在其添加子设备节点
/* SPI device example */
&spi1 {
     my_spi_device: temp-sensor@0 {
             compatible = "...";
             label = "TEMP_SENSOR_0";
             /* reg is the chip select number, if needed;
              * If present, it must match the node's unit address. */
             reg = <0>;
             /* Configure other SPI device properties as needed.
              * Find your device's DT binding for details. */
             spi-max-frequency = <4000000>;
     };
};

```

```
