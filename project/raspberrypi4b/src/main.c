/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      main.c
 * @brief     main source file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2025-10-25
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2025/10/25  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "driver_stcc4_read_test.h"
#include "driver_stcc4_register_test.h"
#include "driver_stcc4_basic.h"
#include "driver_stcc4_shot.h"
#include <getopt.h>
#include <stdlib.h>

/**
 * @brief     stcc4 full function
 * @param[in] argc arg numbers
 * @param[in] **argv arg address
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 5 param is invalid
 * @note      none
 */
uint8_t stcc4(uint8_t argc, char **argv)
{
    int c;
    int longindex = 0;
    const char short_options[] = "hipe:t:";
    const struct option long_options[] =
    {
        {"help", no_argument, NULL, 'h'},
        {"information", no_argument, NULL, 'i'},
        {"port", no_argument, NULL, 'p'},
        {"example", required_argument, NULL, 'e'},
        {"test", required_argument, NULL, 't'},
        {"addr", required_argument, NULL, 1},
        {"times", required_argument, NULL, 2},
        {NULL, 0, NULL, 0},
    };
    char type[33] = "unknown";
    uint32_t times = 3;
    stcc4_address_t address = STCC4_ADDRESS_0;
    
    /* if no params */
    if (argc == 1)
    {
        /* goto the help */
        goto help;
    }
    
    /* init 0 */
    optind = 0;
    
    /* parse */
    do
    {
        /* parse the args */
        c = getopt_long(argc, argv, short_options, long_options, &longindex);
        
        /* judge the result */
        switch (c)
        {
            /* help */
            case 'h' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "h");
                
                break;
            }
            
            /* information */
            case 'i' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "i");
                
                break;
            }
            
            /* port */
            case 'p' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "p");
                
                break;
            }
            
            /* example */
            case 'e' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "e_%s", optarg);
                
                break;
            }
            
            /* test */
            case 't' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "t_%s", optarg);
                
                break;
            }
            
            /* address */
            case 1 :
            {
                if (strcmp(optarg, "0") == 0)
                {
                    address = STCC4_ADDRESS_0;
                }
                else if (strcmp(optarg, "1") == 0)
                {
                    address = STCC4_ADDRESS_1;
                }
                else
                {
                    return 5;
                }
                
                break;
            } 
            
            /* running times */
            case 2 :
            {
                /* set the times */
                times = atol(optarg);
                
                break;
            } 
            
            /* the end */
            case -1 :
            {
                break;
            }
            
            /* others */
            default :
            {
                return 5;
            }
        }
    } while (c != -1);

    /* run functions */
    if (strcmp("t_reg", type) == 0)
    {
        /* run reg test */
        if (stcc4_register_test(address) != 0)
        {
            return 1;
        }
        
        return 0;
    }
    else if (strcmp("t_read", type) == 0)
    {
        /* run read test */
        if (stcc4_read_test(address, times) != 0)
        {
            return 1;
        }
        
        return 0;
    }
    else if (strcmp("e_read", type) == 0)
    {
        uint8_t res;
        uint32_t i;
        int16_t co2_ppm;
        float temperature;
        float humidity;
        
        /* basic init */
        res = stcc4_basic_init(address);
        if (res != 0)
        {
            return 1;
        }
        
        /* loop */
        for (i = 0; i < times; i++)
        {
            /* delay 1000ms */
            stcc4_interface_delay_ms(1000);
            
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
        }
        
        /* basic deinit */
        (void)stcc4_basic_deinit();
        
        return 0;
    }
    else if (strcmp("e_shot", type) == 0)
    {
        uint8_t res;
        uint32_t i;
        int16_t co2_ppm;
        float temperature;
        float humidity;
        
        /* shot init */
        res = stcc4_shot_init(address);
        if (res != 0)
        {
            return 1;
        }
        
        /* loop */
        for (i = 0; i < times; i++)
        {
            /* delay 1000ms */
            stcc4_interface_delay_ms(1000);
            
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
        }
        
        /* shot deinit */
        (void)stcc4_shot_deinit();
        
        return 0;
    }
    else if (strcmp("e_number", type) == 0)
    {
        uint8_t res;
        uint32_t product_id;
        uint8_t unique_serial_number[8];
        
        /* shot init */
        res = stcc4_shot_init(address);
        if (res != 0)
        {
            return 1;
        }
        
        /* get product id */
        res = stcc4_shot_get_product_id(&product_id, unique_serial_number);
        if (res != 0)
        {
            (void)stcc4_shot_deinit();
            
            return 1;
        }
        
        /* output */
        stcc4_interface_debug_print("stcc4: product id is 0x%08X.\n", product_id);
        stcc4_interface_debug_print("stcc4: serial number is 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X.\n", 
                                    unique_serial_number[0], unique_serial_number[1], unique_serial_number[2], unique_serial_number[3],
                                    unique_serial_number[4], unique_serial_number[5], unique_serial_number[6], unique_serial_number[7]);
        
        /* shot deinit */
        (void)stcc4_shot_deinit();
        
        return 0;
    }
    else if (strcmp("e_wake-up", type) == 0)
    {
        uint8_t res;
        
        /* shot init */
        res = stcc4_shot_init(address);
        if (res != 0)
        {
            return 1;
        }
        
        /* wake up */
        res = stcc4_shot_wake_up();
        if (res != 0)
        {
            (void)stcc4_shot_deinit();
            
            return 1;
        }
        
        /* output */
        stcc4_interface_debug_print("stcc4: wake up.\n");
        
        /* shot deinit */
        (void)stcc4_shot_deinit();
        
        return 0;
    }
    else if (strcmp("e_power-down", type) == 0)
    {
        uint8_t res;
        
        /* shot init */
        res = stcc4_shot_init(address);
        if (res != 0)
        {
            return 1;
        }
        
        /* power down */
        res = stcc4_shot_power_down();
        if (res != 0)
        {
            (void)stcc4_shot_deinit();
            
            return 1;
        }
        
        /* wake up */
        res = stcc4_shot_wake_up();
        if (res != 0)
        {
            (void)stcc4_shot_deinit();
            
            return 1;
        }
        
        /* output */
        stcc4_interface_debug_print("stcc4: power down.\n");
        
        /* shot deinit */
        (void)stcc4_shot_deinit();
        
        return 0;
    }
    else if (strcmp("h", type) == 0)
    {
        help:
        stcc4_interface_debug_print("Usage:\n");
        stcc4_interface_debug_print("  stcc4 (-i | --information)\n");
        stcc4_interface_debug_print("  stcc4 (-h | --help)\n");
        stcc4_interface_debug_print("  stcc4 (-p | --port)\n");
        stcc4_interface_debug_print("  stcc4 (-t reg | --test=reg) [--addr=<0 | 1>]\n");
        stcc4_interface_debug_print("  stcc4 (-t read | --test=read) [--addr=<0 | 1>] [--times=<num>]\n");
        stcc4_interface_debug_print("  stcc4 (-e read | --example=read) [--addr=<0 | 1>] [--times=<num>]\n");
        stcc4_interface_debug_print("  stcc4 (-e shot | --example=shot) [--addr=<0 | 1>] [--times=<num>]\n");
        stcc4_interface_debug_print("  stcc4 (-e wake-up | --example=wake-up) [--addr=<0 | 1>]\n");
        stcc4_interface_debug_print("  stcc4 (-e power-down | --example=power-down) [--addr=<0 | 1>]\n");
        stcc4_interface_debug_print("  stcc4 (-e number | --example=number) [--addr=<0 | 1>]\n");
        stcc4_interface_debug_print("\n");
        stcc4_interface_debug_print("Options:\n");
        stcc4_interface_debug_print("      --addr=<0 | 1>    Set the chip address.([default: 0])\n");
        stcc4_interface_debug_print("  -e <read | shot | wake-up | power-down | number>, --example=<read | shot | wake-up | power-down | number>\n");
        stcc4_interface_debug_print("                        Run the driver example.\n");
        stcc4_interface_debug_print("  -h, --help            Show the help.\n");
        stcc4_interface_debug_print("  -i, --information     Show the chip information.\n");
        stcc4_interface_debug_print("  -p, --port            Display the pin connections of the current board.\n");
        stcc4_interface_debug_print("  -t <reg | read>, --test=<reg | read>\n");
        stcc4_interface_debug_print("                        Run the driver test.\n");
        stcc4_interface_debug_print("      --times=<num>     Set the running times.([default: 3])\n");
        
        return 0;
    }
    else if (strcmp("i", type) == 0)
    {
        stcc4_info_t info;
        
        /* print stcc4 info */
        stcc4_info(&info);
        stcc4_interface_debug_print("stcc4: chip is %s.\n", info.chip_name);
        stcc4_interface_debug_print("stcc4: manufacturer is %s.\n", info.manufacturer_name);
        stcc4_interface_debug_print("stcc4: interface is %s.\n", info.interface);
        stcc4_interface_debug_print("stcc4: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        stcc4_interface_debug_print("stcc4: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        stcc4_interface_debug_print("stcc4: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        stcc4_interface_debug_print("stcc4: max current is %0.2fmA.\n", info.max_current_ma);
        stcc4_interface_debug_print("stcc4: max temperature is %0.1fC.\n", info.temperature_max);
        stcc4_interface_debug_print("stcc4: min temperature is %0.1fC.\n", info.temperature_min);
        
        return 0;
    }
    else if (strcmp("p", type) == 0)
    {
        /* print pin connection */
        stcc4_interface_debug_print("stcc4: SCL connected to GPIO3(BCM).\n");
        stcc4_interface_debug_print("stcc4: SDA connected to GPIO2(BCM).\n");
        
        return 0;
    }
    else
    {
        return 5;
    }
}

/**
 * @brief     main function
 * @param[in] argc arg numbers
 * @param[in] **argv arg address
 * @return    status code
 *             - 0 success
 * @note      none
 */
int main(uint8_t argc, char **argv)
{
    uint8_t res;

    res = stcc4(argc, argv);
    if (res == 0)
    {
        /* run success */
    }
    else if (res == 1)
    {
        stcc4_interface_debug_print("stcc4: run failed.\n");
    }
    else if (res == 5)
    {
        stcc4_interface_debug_print("stcc4: param is invalid.\n");
    }
    else
    {
        stcc4_interface_debug_print("stcc4: unknown status code.\n");
    }

    return 0;
}
