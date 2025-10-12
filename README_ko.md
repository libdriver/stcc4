[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver STCC4

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/stcc4/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

STCC4는 실내 공기질 측정을 위한 센시리온의 차세대 소형 CO2 센서입니다. 열전도도 감지 원리와 센시리온의 CMOSens® 기술을 기반으로 하는 STCC4는 탁월한 비용 효율성과 폼팩터로 실내 대기 중의 CO2 가스 농도를 모니터링합니다. SMD 어셈블리와 테이프 앤 릴 패키징을 통해 대량 생산 시 비용 및 공간 효율적인 통합이 가능합니다. STCC4는 100% 공장 교정되었으며, 외부 SHT4x 센서를 통해 습도 및 온도에 따른 CO2 출력을 자동으로 보정합니다.

LibDriver STCC4는 LibDriver가 출시한 STCC4의 전체 기능 드라이버입니다. CO2 판독, 온도 판독, 습도 판독 및 기타 기능을 제공합니다. LibDriver는 MISRA를 준수합니다.

### 콘텐츠

  - [설명](#설명)
  - [설치](#설치)
  - [사용](#사용)
    - [example basic](#example-basic)
    - [example shot](#example-shot)
  - [문서](#문서)
  - [기고](#기고)
  - [저작권](#저작권)
  - [문의하기](#문의하기)

### 설명

/src 디렉토리에는 LibDriver STCC4의 소스 파일이 포함되어 있습니다.

/interface 디렉토리에는 LibDriver STCC4용 플랫폼 독립적인 IIC버스 템플릿이 포함되어 있습니다.

/test 디렉토리에는 LibDriver STCC4드라이버 테스트 프로그램이 포함되어 있어 칩의 필요한 기능을 간단히 테스트할 수 있습니다.

/example 디렉토리에는 LibDriver STCC4프로그래밍 예제가 포함되어 있습니다.

/doc 디렉토리에는 LibDriver STCC4오프라인 문서가 포함되어 있습니다.

/datasheet 디렉토리에는 STCC4데이터시트가 있습니다.

/project 디렉토리에는 일반적으로 사용되는 Linux 및 마이크로컨트롤러 개발 보드의 프로젝트 샘플이 포함되어 있습니다. 모든 프로젝트는 디버깅 방법으로 셸 스크립트를 사용하며, 자세한 내용은 각 프로젝트의 README.md를 참조하십시오.

/misra 에는 LibDriver misra 코드 검색 결과가 포함됩니다.

### 설치

/interface 디렉토리에서 플랫폼 독립적인IIC 버스 템플릿을 참조하여 지정된 플랫폼에 대한 IIC버스 드라이버를 완성하십시오.

/src 디렉터리, 플랫폼용 인터페이스 드라이버 및 자체 드라이버를 프로젝트에 추가합니다. 기본 예제 드라이버를 사용하려면 /example 디렉터리를 프로젝트에 추가합니다.

### 사용

/example 디렉터리의 예제를 참조하여 자신만의 드라이버를 완성할 수 있습니다. 기본 프로그래밍 예제를 사용하려는 경우 사용 방법은 다음과 같습니다.

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
### 문서

온라인 문서: [https://www.libdriver.com/docs/stcc4/index.html](https://www.libdriver.com/docs/stcc4/index.html).

오프라인 문서: /doc/html/index.html.

### 기고

CONTRIBUTING.md 를 참조하십시오.

### 저작권

저작권 (c) 2015 - 지금 LibDriver 판권 소유

MIT 라이선스(MIT)

이 소프트웨어 및 관련 문서 파일("소프트웨어")의 사본을 얻은 모든 사람은 이에 따라 무제한 사용, 복제, 수정, 통합, 출판, 배포, 2차 라이선스를 포함하여 소프트웨어를 처분할 수 있는 권리가 부여됩니다. 소프트웨어의 사본에 대한 라이선스 및/또는 판매, 그리고 소프트웨어가 위와 같이 배포된 사람의 권리에 대한 2차 라이선스는 다음 조건에 따릅니다.

위의 저작권 표시 및 이 허가 표시는 이 소프트웨어의 모든 사본 또는 내용에 포함됩니다.

이 소프트웨어는 상품성, 특정 목적에의 적합성 및 비침해에 대한 보증을 포함하되 이에 국한되지 않는 어떠한 종류의 명시적 또는 묵시적 보증 없이 "있는 그대로" 제공됩니다. 어떤 경우에도 저자 또는 저작권 소유자는 계약, 불법 행위 또는 기타 방식에 관계없이 소프트웨어 및 기타 소프트웨어 사용으로 인해 발생하거나 이와 관련하여 발생하는 청구, 손해 또는 기타 책임에 대해 책임을 지지 않습니다.

### 문의하기

연락주세요lishifenging@outlook.com.