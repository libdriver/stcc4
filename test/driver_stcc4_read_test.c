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
 * @file      driver_stcc4_read_test.c
 * @brief     driver stcc4 read test source file
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

static stcc4_handle_t gs_handle;        /**< stcc4 handle */

/**
 * @brief     read test
 * @param[in] address address pin
 * @param[in] times test times
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t stcc4_read_test(stcc4_address_t address, uint32_t times)
{
    uint8_t res;
    uint32_t i;
    stcc4_info_t info;
    
    /* link functions */
    DRIVER_STCC4_LINK_INIT(&gs_handle, stcc4_handle_t);
    DRIVER_STCC4_LINK_IIC_INIT(&gs_handle, stcc4_interface_iic_init);
    DRIVER_STCC4_LINK_IIC_DEINIT(&gs_handle, stcc4_interface_iic_deinit);
    DRIVER_STCC4_LINK_IIC_WRITE_COMMAND(&gs_handle, stcc4_interface_iic_write_cmd);
    DRIVER_STCC4_LINK_IIC_READ_COMMAND(&gs_handle, stcc4_interface_iic_read_cmd);
    DRIVER_STCC4_LINK_DELAY_MS(&gs_handle, stcc4_interface_delay_ms);
    DRIVER_STCC4_LINK_DEBUG_PRINT(&gs_handle, stcc4_interface_debug_print);
    
    /* stcc4 info */
    res = stcc4_info(&info);
    if (res != 0)
    {
        stcc4_interface_debug_print("stcc4: get info failed.\n");
       
        return 1;
    }
    else
    {
        /* print chip information */
        stcc4_interface_debug_print("stcc4: chip is %s.\n", info.chip_name);
        stcc4_interface_debug_print("stcc4: manufacturer is %s.\n", info.manufacturer_name);
        stcc4_interface_debug_print("stcc4: interface is %s.\n", info.interface);
        stcc4_interface_debug_print("stcc4: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        stcc4_interface_debug_print("stcc4: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        stcc4_interface_debug_print("stcc4: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        stcc4_interface_debug_print("stcc4: max current is %0.2fmA.\n", info.max_current_ma);
        stcc4_interface_debug_print("stcc4: max temperature is %0.1fC.\n", info.temperature_max);
        stcc4_interface_debug_print("stcc4: min temperature is %0.1fC.\n", info.temperature_min);
    }
    
    /* start read test */
    stcc4_interface_debug_print("stcc4: start read test.\n");
    
    /* set address pin */
    res = stcc4_set_address_pin(&gs_handle, address);
    if (res != 0)
    {
        stcc4_interface_debug_print("stcc4: set address pin failed.\n");
        
        return 1;
    }
    
    /* stcc4 init */
    res = stcc4_init(&gs_handle);
    if (res != 0)
    {
        stcc4_interface_debug_print("stcc4: init failed.\n");
        
        return 1;
    }
    
    /* continuous read test */
    stcc4_interface_debug_print("stcc4: continuous read test.\n");
    
    /* start continuous measurement */
    res = stcc4_start_continuous_measurement(&gs_handle);
    if (res != 0)
    {
        stcc4_interface_debug_print("stcc4: start continuous measurement failed.\n");
        (void)stcc4_deinit(&gs_handle);
        
        return 1;
    }
    
    /* loop */
    for (i = 0; i < times; i++)
    {
        int16_t co2_raw;
        int16_t co2_ppm;
        uint16_t temperature_raw;
        float temperature_s;
        uint16_t humidity_raw;
        float humidity_s;
        uint16_t sensor_status;
        
        /* delay 1000ms */
        stcc4_interface_delay_ms(1000);
        
        /* read data */
        res = stcc4_read(&gs_handle, &co2_raw, &co2_ppm,
                         &temperature_raw, &temperature_s,
                         &humidity_raw, &humidity_s, &sensor_status);
        if (res != 0)
        {
            stcc4_interface_debug_print("stcc4: read failed.\n");
            (void)stcc4_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        stcc4_interface_debug_print("stcc4: co2 is %02dppm.\n", co2_ppm);
        stcc4_interface_debug_print("stcc4: temperature is %0.2fC.\n", temperature_s);
        stcc4_interface_debug_print("stcc4: humidity is %0.2f%%.\n", humidity_s);
        stcc4_interface_debug_print("stcc4: sensor status is 0x%04X.\n", sensor_status);
    }
    
    /* stop continuous measurement */
    res = stcc4_stop_continuous_measurement(&gs_handle);
    if (res != 0)
    {
        stcc4_interface_debug_print("stcc4: stop continuous measurement failed.\n");
        (void)stcc4_deinit(&gs_handle);
        
        return 1;
    }
    
    /* measure single shot test */
    stcc4_interface_debug_print("stcc4: measure single shot test.\n");
    
    /* loop */
    for (i = 0; i < times; i++)
    {
        int16_t co2_raw;
        int16_t co2_ppm;
        uint16_t temperature_raw;
        float temperature_s;
        uint16_t humidity_raw;
        float humidity_s;
        uint16_t sensor_status;
        
        /* delay 1000ms */
        stcc4_interface_delay_ms(1000);
        
        /* measure single shot */
        res = stcc4_measure_single_shot(&gs_handle);
        if (res != 0)
        {
            stcc4_interface_debug_print("stcc4: measure single shot failed.\n");
            (void)stcc4_deinit(&gs_handle);
            
            return 1;
        }
        
        /* read data */
        res = stcc4_read(&gs_handle, &co2_raw, &co2_ppm,
                         &temperature_raw, &temperature_s,
                         &humidity_raw, &humidity_s, &sensor_status);
        if (res != 0)
        {
            stcc4_interface_debug_print("stcc4: read failed.\n");
            (void)stcc4_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        stcc4_interface_debug_print("stcc4: co2 is %02dppm.\n", co2_ppm);
        stcc4_interface_debug_print("stcc4: temperature is %0.2fC.\n", temperature_s);
        stcc4_interface_debug_print("stcc4: humidity is %0.2f%%.\n", humidity_s);
        stcc4_interface_debug_print("stcc4: sensor status is 0x%04X.\n", sensor_status);
    }
    
    /* finish read test */
    stcc4_interface_debug_print("stcc4: finish read test.\n");
    (void)stcc4_deinit(&gs_handle);

    return 0;
}
