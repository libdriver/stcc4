[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver STCC4

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/stcc4/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

STCC4は、Sensirionの次世代小型CO2センサーで、室内空気質アプリケーションに最適です。熱伝導率センシング原理とSensirionのCMOSens®テクノロジーをベースとしたSTCC4は、比類のないコスト効率と小型フォームファクタで、室内空気中のCO2ガス濃度をモニタリングできます。SMDアセンブリとテープ＆リールパッケージにより、STCC4は大量生産アプリケーションにおいて、コスト効率と省スペース性に優れた実装が可能です。STCC4は工場で100%キャリブレーション済みで、外付けのSHT4xセンサーを介して湿度と温度のCO2出力を自動補正します。

LibDriver STCC4 は、LibDriver によって起動された STCC4 のフル機能ドライバーです。CO2 読み取り、温度読み取り、湿度読み取りおよびその他の機能を提供します。 LibDriver は MISRA に準拠しています。

### 目次

  - [説明](#説明)
  - [インストール](#インストール)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example shot](#example-shot)
  - [ドキュメント](#ドキュメント)
  - [貢献](#貢献)
  - [著作権](#著作権)
  - [連絡して](#連絡して)

### 説明

/ srcディレクトリには、LibDriver STCC4のソースファイルが含まれています。

/ interfaceディレクトリには、LibDriver STCC4用のプラットフォームに依存しないIICバステンプレートが含まれています。

/ testディレクトリには、チップの必要な機能を簡単にテストできるLibDriver STCC4ドライバーテストプログラムが含まれています。

/ exampleディレクトリには、LibDriver STCC4プログラミング例が含まれています。

/ docディレクトリには、LibDriver STCC4オフラインドキュメントが含まれています。

/ datasheetディレクトリには、STCC4データシートが含まれています。

/ projectディレクトリには、一般的に使用されるLinuxおよびマイクロコントローラー開発ボードのプロジェクトサンプルが含まれています。 すべてのプロジェクトは、デバッグ方法としてシェルスクリプトを使用しています。詳細については、各プロジェクトのREADME.mdを参照してください。

/ misraはLibDriver misraコードスキャン結果を含む。

### インストール

/ interfaceディレクトリにあるプラットフォームに依存しないIICバステンプレートを参照して、指定したプラットフォームのIICバスドライバを完成させます。

/src ディレクトリ、プラットフォームのインターフェイス ドライバー、および独自のドライバーをプロジェクトに追加します。デフォルトのサンプル ドライバーを使用する場合は、/example ディレクトリをプロジェクトに追加します。

### 使用

/example ディレクトリ内のサンプルを参照して、独自のドライバーを完成させることができます。 デフォルトのプログラミング例を使用したい場合の使用方法は次のとおりです。

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

### ドキュメント

オンラインドキュメント: [https://www.libdriver.com/docs/stcc4/index.html](https://www.libdriver.com/docs/stcc4/index.html)。

オフラインドキュメント: /doc/html/index.html。

### 貢献

CONTRIBUTING.mdを参照してください。

### 著作権

著作権（c）2015-今 LibDriver 全著作権所有

MITライセンス（MIT）

このソフトウェアおよび関連するドキュメントファイル（「ソフトウェア」）のコピーを取得した人は、無制限の使用、複製、変更、組み込み、公開、配布、サブライセンスを含む、ソフトウェアを処分する権利を制限なく付与されます。ソフトウェアのライセンスおよび/またはコピーの販売、および上記のようにソフトウェアが配布された人の権利のサブライセンスは、次の条件に従うものとします。

上記の著作権表示およびこの許可通知は、このソフトウェアのすべてのコピーまたは実体に含まれるものとします。

このソフトウェアは「現状有姿」で提供され、商品性、特定目的への適合性、および非侵害の保証を含むがこれらに限定されない、明示または黙示を問わず、いかなる種類の保証もありません。 いかなる場合も、作者または著作権所有者は、契約、不法行為、またはその他の方法で、本ソフトウェアおよび本ソフトウェアの使用またはその他の廃棄に起因または関連して、請求、損害、またはその他の責任を負わないものとします。

### 連絡して

お問い合わせくださいlishifenging@outlook.com。