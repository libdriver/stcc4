[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver STCC4

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/stcc4/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE) 

Der STCC4 ist Sensirions Miniatur-CO2-Sensor der nächsten Generation für Anwendungen zur Messung der Raumluftqualität. Basierend auf dem Wärmeleitfähigkeits-Sensorprinzip und der CMOSens®-Technologie von Sensirion ermöglicht der STCC4 die Überwachung der CO2-Konzentration in der Raumluft zu unübertroffener Kosteneffizienz und in einem kompakten Format. SMD-Montage und Tape-&-Reel-Verpackung ermöglichen eine kostengünstige und platzsparende Integration des STCC4 in Großserienanwendungen. Der STCC4 ist werkseitig vollständig kalibriert und ermöglicht die automatische On-Board-Kompensation der CO2-Ausgabe für Luftfeuchtigkeit und Temperatur durch einen externen SHT4x-Sensor.

LibDriver STCC4 ist ein voll funktionsfähiger Treiber von STCC4, der von LibDriver gestartet wurde. Er bietet CO2-Messung, Temperaturmessung, Luftfeuchtigkeitsmessung und andere Funktionen. LibDriver ist MISRA-kompatibel.

### Inhaltsverzeichnis

  - [Anweisung](#Anweisung)
  - [Installieren](#Installieren)
  - [Nutzung](#Nutzung)
    - [example basic](#example-basic)
    - [example shot](#example-shot)
  - [Dokument](#Dokument)
  - [Beitrag](#Beitrag)
  - [Lizenz](#Lizenz)
  - [Kontaktieren Sie uns](#Kontaktieren-Sie-uns)

### Anweisung

/src enthält LibDriver STCC4-Quelldateien.

/interface enthält die plattformunabhängige Vorlage LibDriver STCC4 IIC.

/test enthält den Testcode des LibDriver STCC4-Treibers und dieser Code kann die erforderliche Funktion des Chips einfach testen.

/example enthält LibDriver STCC4-Beispielcode.

/doc enthält das LibDriver STCC4-Offlinedokument.

/Datenblatt enthält STCC4-Datenblatt.

/project enthält den allgemeinen Beispielcode für Linux- und MCU-Entwicklungsboards. Alle Projekte verwenden das Shell-Skript, um den Treiber zu debuggen, und die detaillierten Anweisungen finden Sie in der README.md jedes Projekts.

/misra enthält die Ergebnisse des LibDriver MISRA Code Scans.

### Installieren

Verweisen Sie auf eine plattformunabhängige IIC-Schnittstellenvorlage und stellen Sie Ihren Plattform-IIC-Treiber fertig.

Fügen Sie das Verzeichnis /src, den Schnittstellentreiber für Ihre Plattform und Ihre eigenen Treiber zu Ihrem Projekt hinzu. Wenn Sie die Standardbeispieltreiber verwenden möchten, fügen Sie das Verzeichnis /example zu Ihrem Projekt hinzu.

### Nutzung

Sie können auf die Beispiele im Verzeichnis /example zurückgreifen, um Ihren eigenen Treiber zu vervollständigen. Wenn Sie die Standardprogrammierbeispiele verwenden möchten, erfahren Sie hier, wie Sie diese verwenden.

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
### Dokument

Online-Dokumente: [https://www.libdriver.com/docs/stcc4/index.html](https://www.libdriver.com/docs/stcc4/index.html).

Offline-Dokumente: /doc/html/index.html.

### Beitrag

Bitte beachten Sie CONTRIBUTING.md.

### Lizenz

Urheberrechte © (c) 2015 - Gegenwart LibDriver Alle Rechte vorbehalten



Die MIT-Lizenz (MIT)



Hiermit wird jeder Person kostenlos die Erlaubnis erteilt, eine Kopie zu erhalten

dieser Software und zugehörigen Dokumentationsdateien (die „Software“) zu behandeln

in der Software ohne Einschränkung, einschließlich, aber nicht beschränkt auf die Rechte

zu verwenden, zu kopieren, zu modifizieren, zusammenzuführen, zu veröffentlichen, zu verteilen, unterzulizenzieren und/oder zu verkaufen

Kopien der Software und Personen, denen die Software gehört, zu gestatten

dazu eingerichtet werden, unter folgenden Bedingungen:



Der obige Urheberrechtshinweis und dieser Genehmigungshinweis müssen in allen enthalten sein

Kopien oder wesentliche Teile der Software.



DIE SOFTWARE WIRD "WIE BESEHEN" BEREITGESTELLT, OHNE JEGLICHE GEWÄHRLEISTUNG, AUSDRÜCKLICH ODER

STILLSCHWEIGEND, EINSCHLIESSLICH, ABER NICHT BESCHRÄNKT AUF DIE GEWÄHRLEISTUNG DER MARKTGÄNGIGKEIT,

EIGNUNG FÜR EINEN BESTIMMTEN ZWECK UND NICHTVERLETZUNG VON RECHTEN DRITTER. IN KEINEM FALL DARF DAS

AUTOREN ODER URHEBERRECHTSINHABER HAFTEN FÜR JEGLICHE ANSPRÜCHE, SCHÄDEN ODER ANDERE

HAFTUNG, OB AUS VERTRAG, DELIKT ODER ANDERWEITIG, ENTSTEHEND AUS,

AUS ODER IM ZUSAMMENHANG MIT DER SOFTWARE ODER DER VERWENDUNG ODER ANDEREN HANDLUNGEN MIT DER

SOFTWARE.

### Kontaktieren Sie uns

Bitte senden Sie eine E-Mail an lishifenging@outlook.com.