### 1. Chip

#### 1.1 Chip Info

Chip Name: STM32F407ZGT6.

Extern Oscillator: 8MHz.

UART Pin: TX/RX PA9/PA10.

IIC Pin: SCL/SDA PB8/PB9.

### 2. Development and Debugging

#### 2.1 Integrated Development Environment

LibDriver provides both Keil and IAR integrated development environment projects.

MDK is the Keil ARM project and your Keil version must be 5 or higher.Keil ARM project needs STMicroelectronics STM32F4 Series Device Family Pack and you can download from https://www.keil.com/dd2/stmicroelectronics/stm32f407zgtx.

EW is the IAR ARM project and your IAR version must be 9 or higher.

#### 2.2 Serial Port Parameter

Baud Rate: 115200.

Data Bits : 8.

Stop Bits: 1.

Parity: None.

Flow Control: None.

#### 2.3 Serial Port Assistant

We use '\n' to wrap lines.If your serial port assistant displays exceptions (e.g. the displayed content does not divide lines), please modify the configuration of your serial port assistant or replace one that supports '\n' parsing.

### 3. STCC4

#### 3.1 Command Instruction

1. Show stcc4 chip and driver information.

   ```shell
   stcc4 (-i | --information)
   ```

2. Show stcc4 help.

   ```shell
   stcc4 (-h | --help)
   ```

3. Show stcc4 pin connections of the current board.

   ```shell
   stcc4 (-p | --port)
   ```

4. Run stcc4 register test.

   ```shell
   stcc4 (-t reg | --test=reg) [--addr=<0 | 1>]
   ```

5. Run stcc4 read test, num is test times.

   ```shell
   stcc4 (-t read | --test=read) [--addr=<0 | 1>] [--times=<num>]
   ```

6. Run stcc4 read function, num is read times.

   ```shell
   stcc4 (-e read | --example=read) [--addr=<0 | 1>] [--times=<num>]
   ```

7. Run stcc4 shot function, num is read times.

   ```shell
   stcc4 (-e shot | --example=shot) [--addr=<0 | 1>] [--times=<num>]
   ```

8. Run stcc4 wake up function.

   ```shell
   stcc4 (-e wake-up | --example=wake-up) [--addr=<0 | 1>]
   ```

9. Run stcc4 power down function.

   ```shell
   stcc4 (-e power-down | --example=power-down) [--addr=<0 | 1>]
   ```

10. Run stcc4 number function.

    ```shell
    stcc4 (-e number | --example=number) [--addr=<0 | 1>]
    ```

#### 3.2 Command Example

```shell
stcc4 -i

stcc4: chip is Sensirion STCC4.
stcc4: manufacturer is Sensirion.
stcc4: interface is IIC.
stcc4: driver version is 1.0.
stcc4: min supply voltage is 2.7V.
stcc4: max supply voltage is 5.5V.
stcc4: max current is 4.20mA.
stcc4: max temperature is 85.0C.
stcc4: min temperature is -40.0C.
```

```shell
stcc4 -p

stcc4: SCL connected to GPIOB PIN8.
stcc4: SDA connected to GPIOB PIN9.
```

```shell
stcc4 -t reg --addr=0

stcc4: chip is Sensirion STCC4.
stcc4: manufacturer is Sensirion.
stcc4: interface is IIC.
stcc4: driver version is 1.0.
stcc4: min supply voltage is 2.7V.
stcc4: max supply voltage is 5.5V.
stcc4: max current is 4.20mA.
stcc4: max temperature is 85.0C.
stcc4: min temperature is -40.0C.
stcc4: start register test.
stcc4: stcc4_set_address_pin/stcc4_get_address_pin test.
stcc4: set address pin 0.
stcc4: check address pin ok.
stcc4: set address pin 1.
stcc4: check address pin ok.
stcc4: stcc4_get_product_id test.
stcc4: product id is 0x0901018A.
stcc4: serial number is 0x23 0x03 0x7F 0x21 0xF0 0x7E 0x59 0xC8.
stcc4: stcc4_set_rht_compensation test.
stcc4: check rht compensation ok.
stcc4: stcc4_set_pressure_compensation test.
stcc4: check pressure compensation ok.
stcc4: stcc4_perform_conditioning test.
stcc4: check perform conditionin ok.
stcc4: stcc4_perform_self_test test.
stcc4: self test result is 0x0000.
stcc4: check perform self test ok.
stcc4: stcc4_perform_forced_recalibration test.
stcc4: correct co2 is 0xFFFF.
stcc4: check perform forced recalibration ok.
stcc4: stcc4_perform_factory_reset test.
stcc4: check perform factory reset ok.
stcc4: stcc4_enable_testing_mode test.
stcc4: check enable testing mode ok.
stcc4: stcc4_disable_testing_mode test.
stcc4: check disable testing mode ok.
stcc4: stcc4_frc_co2_convert_to_register/stcc4_frc_co2_convert_to_data test.
stcc4: set frc co2 16.00ppm.
stcc4: check frc co2 16.00ppm.
stcc4: stcc4_humidity_convert_to_register/stcc4_humidity_convert_to_data test.
stcc4: set humidity 58.30%.
stcc4: check humidity 58.30%.
stcc4: stcc4_temperature_convert_to_register/stcc4_temperature_convert_to_data test.
stcc4: set temperature 74.10C.
stcc4: check temperature 74.10C.
stcc4: stcc4_pressure_convert_to_register/stcc4_pressure_convert_to_data test.
stcc4: set pressure 6760.00pa.
stcc4: check pressure 6760.00pa.
stcc4: stcc4_enter_sleep_mode test.
stcc4: check enter sleep mode ok.
stcc4: stcc4_exit_sleep_mode test.
stcc4: check exit sleep mode ok.
stcc4: stcc4_perform_soft_reset test.
stcc4: check perform soft reset ok.
stcc4: finish register test.
```

```shell
stcc4 -t read --addr=0 --times=3

stcc4: chip is Sensirion STCC4.
stcc4: manufacturer is Sensirion.
stcc4: interface is IIC.
stcc4: driver version is 1.0.
stcc4: min supply voltage is 2.7V.
stcc4: max supply voltage is 5.5V.
stcc4: max current is 4.20mA.
stcc4: max temperature is 85.0C.
stcc4: min temperature is -40.0C.
stcc4: start read test.
stcc4: continuous read test.
stcc4: co2 is 390ppm.
stcc4: temperature is 27.54C.
stcc4: humidity is 52.80%.
stcc4: sensor status is 0x0000.
stcc4: co2 is 390ppm.
stcc4: temperature is 27.55C.
stcc4: humidity is 52.79%.
stcc4: sensor status is 0x0000.
stcc4: co2 is 390ppm.
stcc4: temperature is 27.59C.
stcc4: humidity is 52.80%.
stcc4: sensor status is 0x0000.
stcc4: measure single shot test.
stcc4: co2 is 390ppm.
stcc4: temperature is 27.58C.
stcc4: humidity is 52.74%.
stcc4: sensor status is 0x0000.
stcc4: co2 is 390ppm.
stcc4: temperature is 27.58C.
stcc4: humidity is 52.74%.
stcc4: sensor status is 0x0000.
stcc4: co2 is 513ppm.
stcc4: temperature is 27.57C.
stcc4: humidity is 52.71%.
stcc4: sensor status is 0x0000.
stcc4: finish read test.
```

```shell
stcc4 -e read --addr=0 --times=3

stcc4: 1/3.
stcc4: co2 is 390ppm.
stcc4: temperature is 27.59C.
stcc4: humidity is 52.72%.
stcc4: 2/3.
stcc4: co2 is 390ppm.
stcc4: temperature is 27.59C.
stcc4: humidity is 52.68%.
stcc4: 3/3.
stcc4: co2 is 390ppm.
stcc4: temperature is 27.57C.
stcc4: humidity is 52.66%.
```

```shell
stcc4 -e shot --addr=0 --times=3

stcc4: 1/3.
stcc4: co2 is 390ppm.
stcc4: temperature is 27.52C.
stcc4: humidity is 52.74%.
stcc4: 2/3.
stcc4: co2 is 390ppm.
stcc4: temperature is 27.57C.
stcc4: humidity is 52.74%.
stcc4: 3/3.
stcc4: co2 is 382ppm.
stcc4: temperature is 27.57C.
stcc4: humidity is 52.74%.
```

```shell
stcc4 -e wake-up --addr=0

stcc4: wake up.
```

```shell
stcc4 -e power-down --addr=0

stcc4: power down.
```

```shell
stcc4 -e number --addr=0

stcc4: product id is 0x0901018A.
stcc4: serial number is 0x23 0x03 0x7F 0x21 0xF0 0x7E 0x59 0xC8.
```

```shell
stcc4 -h

Usage:
  stcc4 (-i | --information)
  stcc4 (-h | --help)
  stcc4 (-p | --port)
  stcc4 (-t reg | --test=reg) [--addr=<0 | 1>]
  stcc4 (-t read | --test=read) [--addr=<0 | 1>] [--times=<num>]
  stcc4 (-e read | --example=read) [--addr=<0 | 1>] [--times=<num>]
  stcc4 (-e shot | --example=shot) [--addr=<0 | 1>] [--times=<num>]
  stcc4 (-e wake-up | --example=wake-up) [--addr=<0 | 1>]
  stcc4 (-e power-down | --example=power-down) [--addr=<0 | 1>]
  stcc4 (-e number | --example=number) [--addr=<0 | 1>]

Options:
      --addr=<0 | 1>    Set the chip address.([default: 0])
  -e <read | shot | wake-up | power-down | number>, --example=<read | shot | wake-up | power-down | number>
                        Run the driver example.
  -h, --help            Show the help.
  -i, --information     Show the chip information.
  -p, --port            Display the pin connections of the current board.
  -t <reg | read>, --test=<reg | read>
                        Run the driver test.
      --times=<num>     Set the running times.([default: 3])
```

