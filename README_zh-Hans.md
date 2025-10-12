[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver STCC4

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/stcc4/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

STCC4是Sensirion的下一代微型CO2传感器，用于室内空气质量应用。基于热导率传感原理和Sensirion的CMOSens®技术，STCC4能够以无与伦比的成本效益和形状系数监测环境室内空气条件中的CO2气体浓度。SMD组装以及磁带和卷轴封装允许对STCC4进行成本和空间有效的集成，以用于大容量应用。STCC4是100%工厂校准的，并通过外部SHT4x传感器实现CO2输出的湿度和温度自动板载补偿。

LibDriver STCC4是LibDriver推出的STCC4全功能驱动，该驱动提供CO2读取、温度读取、湿度读取等功能并且它符合MISRA标准。

### 目录

  - [说明](#说明)
  - [安装](#安装)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example shot](#example-shot)
  - [文档](#文档)
  - [贡献](#贡献)
  - [版权](#版权)
  - [联系我们](#联系我们)

### 说明

/src目录包含了LibDriver STCC4的源文件。

/interface目录包含了LibDriver STCC4与平台无关的IIC总线模板。

/test目录包含了LibDriver STCC4驱动测试程序，该程序可以简单的测试芯片必要功能。

/example目录包含了LibDriver STCC4编程范例。

/doc目录包含了LibDriver STCC4离线文档。

/datasheet目录包含了STCC4数据手册。

/project目录包含了常用Linux与单片机开发板的工程样例。所有工程均采用shell脚本作为调试方法，详细内容可参考每个工程里面的README.md。

/misra目录包含了LibDriver MISRA代码扫描结果。

### 安装

参考/interface目录下与平台无关的IIC总线模板，完成指定平台的IIC总线驱动。

将/src目录，您使用平台的接口驱动和您开发的驱动加入工程，如果您想要使用默认的范例驱动，可以将/example目录加入您的工程。

### 使用

您可以参考/example目录下的编程范例完成适合您的驱动，如果您想要使用默认的编程范例，以下是它们的使用方法。

#### example basic

```C
#include "driver_stcc4_basic.h"

uint8_t res;
uint32_t i;
uint32_t times = 3;
int16_t co2_ppm;
float temperature;
float humidity;

/* basic init */
res = stcc4_basic_init(STCC4_ADDRESS_0);
if (res != 0)
{
    return 1;
}

...
    
/* loop */
for (i = 0; i < times; i++)
{
    /* delay 5000ms */
    stcc4_interface_delay_ms(5000);

    /* read data */
    res = stcc4_basic_read((int16_t *)&co2_ppm, (float *)&temperature, (float *)&humidity);
    if (res != 0)
    {
        (void)stcc4_basic_deinit();

        return 1;
    }

    /* output */
    stcc4_interface_debug_print("stcc4: %d/%d.\n", (uint32_t)(i + 1), (uint32_t)times);
    stcc4_interface_debug_print("stcc4: co2 is %02dppm.\n", co2_ppm);
    stcc4_interface_debug_print("stcc4: temperature is %0.2fC.\n", temperature);
    stcc4_interface_debug_print("stcc4: humidity is %0.2f%%.\n", humidity);
    
    ...
}

...
    
/* basic deinit */
(void)stcc4_basic_deinit();

return 0;
```
#### example shot

```C
#include "driver_stcc4_shot.h"

uint8_t res;
uint32_t i;
uint32_t times = 3;
int16_t co2_ppm;
float temperature;
float humidity;

/* shot init */
res = stcc4_shot_init(STCC4_ADDRESS_0);
if (res != 0)
{
    return 1;
}

...
    
/* loop */
for (i = 0; i < times; i++)
{
    /* delay 5000ms */
    stcc4_interface_delay_ms(5000);

    /* read data */
    res = stcc4_shot_read((int16_t *)&co2_ppm, (float *)&temperature, (float *)&humidity);
    if (res != 0)
    {
        (void)stcc4_shot_deinit();

        return 1;
    }

    /* output */
    stcc4_interface_debug_print("stcc4: %d/%d.\n", (uint32_t)(i + 1), (uint32_t)times);
    stcc4_interface_debug_print("stcc4: co2 is %02dppm.\n", co2_ppm);
    stcc4_interface_debug_print("stcc4: temperature is %0.2fC.\n", temperature);
    stcc4_interface_debug_print("stcc4: humidity is %0.2f%%.\n", humidity);
    
    ...
}

...
    
/* shot deinit */
(void)stcc4_shot_deinit();

return 0;
```

### 文档

在线文档: [https://www.libdriver.com/docs/stcc4/index.html](https://www.libdriver.com/docs/stcc4/index.html)。

离线文档: /doc/html/index.html。

### 贡献

请参考CONTRIBUTING.md。

### 版权

版权 (c) 2015 - 现在 LibDriver 版权所有

MIT 许可证（MIT）

特此免费授予任何获得本软件副本和相关文档文件（下称“软件”）的人不受限制地处置该软件的权利，包括不受限制地使用、复制、修改、合并、发布、分发、转授许可和/或出售该软件副本，以及再授权被配发了本软件的人如上的权利，须在下列条件下：

上述版权声明和本许可声明应包含在该软件的所有副本或实质成分中。

本软件是“如此”提供的，没有任何形式的明示或暗示的保证，包括但不限于对适销性、特定用途的适用性和不侵权的保证。在任何情况下，作者或版权持有人都不对任何索赔、损害或其他责任负责，无论这些追责来自合同、侵权或其它行为中，还是产生于、源于或有关于本软件以及本软件的使用或其它处置。

### 联系我们

请联系lishifenging@outlook.com。