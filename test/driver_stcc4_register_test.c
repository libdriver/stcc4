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
 * @file      driver_stcc4_register_test.c
 * @brief     driver stcc4 register test source file
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

#include "driver_stcc4_register_test.h"
#include <stdlib.h>

static stcc4_handle_t gs_handle;        /**< stcc4 handle */

/**
 * @brief     register test
 * @param[in] address address pin
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t stcc4_register_test(stcc4_address_t address)
{
    uint8_t res;
    uint16_t reg;
    uint16_t result;
    uint16_t correct_co2;
    uint32_t product_id;
    float f;
    float f_check;
    uint8_t unique_serial_number[8];
    stcc4_address_t address_check;
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
    
    /* start register test */
    stcc4_interface_debug_print("stcc4: start register test.\n");
    
    /* stcc4_set_address_pin/stcc4_get_address_pin test */
    stcc4_interface_debug_print("stcc4: stcc4_set_address_pin/stcc4_get_address_pin test.\n");
    
    /* set address pin 0 */
    res = stcc4_set_address_pin(&gs_handle, STCC4_ADDRESS_0);
    if (res != 0)
    {
        stcc4_interface_debug_print("stcc4: set address pin failed.\n");
        
        return 1;
    }
    stcc4_interface_debug_print("stcc4: set address pin 0.\n");
    res = stcc4_get_address_pin(&gs_handle, &address_check);
    if (res != 0)
    {
        stcc4_interface_debug_print("stcc4: get address pin failed.\n");
        
        return 1;
    }
    stcc4_interface_debug_print("stcc4: check address pin %s.\n", (address_check == STCC4_ADDRESS_0) ? "ok" : "error");
    
    /* set address pin 1 */
    res = stcc4_set_address_pin(&gs_handle, STCC4_ADDRESS_1);
    if (res != 0)
    {
        stcc4_interface_debug_print("stcc4: set address pin failed.\n");
        
        return 1;
    }
    stcc4_interface_debug_print("stcc4: set address pin 1.\n");
    res = stcc4_get_address_pin(&gs_handle, &address_check);
    if (res != 0)
    {
        stcc4_interface_debug_print("stcc4: get address pin failed.\n");
        
        return 1;
    }
    stcc4_interface_debug_print("stcc4: check address pin %s.\n", (address_check == STCC4_ADDRESS_1) ? "ok" : "error");
    
    /* set address pin */
    res = stcc4_set_address_pin(&gs_handle, address);
    if (res != 0)
    {
        stcc4_interface_debug_print("stcc4: set address pin failed.\n");
        
        return 1;
    }
    
    /* init */
    res = stcc4_init(&gs_handle);
    if (res != 0)
    {
        stcc4_interface_debug_print("stcc4: init failed.\n");
        
        return 1;
    }
    
    /* stcc4_get_product_id test */
    stcc4_interface_debug_print("stcc4: stcc4_get_product_id test.\n");
    
    /* get product id */
    res = stcc4_get_product_id(&gs_handle, &product_id, unique_serial_number);
    if (res != 0)
    {
        stcc4_interface_debug_print("stcc4: get product id failed.\n");
        (void)stcc4_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    stcc4_interface_debug_print("stcc4: product id is 0x%08X.\n", product_id);
    stcc4_interface_debug_print("stcc4: serial number is 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X.\n", 
                                unique_serial_number[0], unique_serial_number[1], unique_serial_number[2], unique_serial_number[3],
                                unique_serial_number[4], unique_serial_number[5], unique_serial_number[6], unique_serial_number[7]);
    
    /* stcc4_set_rht_compensation test */
    stcc4_interface_debug_print("stcc4: stcc4_set_rht_compensation test.\n");
    
    reg = rand() % 0xFFFFU;
    res = stcc4_set_rht_compensation(&gs_handle, reg, reg);
    if (res != 0)
    {
        stcc4_interface_debug_print("stcc4: set rht compensation failed.\n");
        (void)stcc4_deinit(&gs_handle);
        
        return 1;
    }
    stcc4_interface_debug_print("stcc4: check rht compensation %s.\n", (res == 0) ? "ok" : "error");
    
    /* stcc4_set_pressure_compensation test */
    stcc4_interface_debug_print("stcc4: stcc4_set_pressure_compensation test.\n");
    
    reg = rand() % 0xFFFFU;
    res = stcc4_set_pressure_compensation(&gs_handle, reg);
    if (res != 0)
    {
        stcc4_interface_debug_print("stcc4: set pressure compensation failed.\n");
        (void)stcc4_deinit(&gs_handle);
        
        return 1;
    }
    stcc4_interface_debug_print("stcc4: check pressure compensation %s.\n", (res == 0) ? "ok" : "error");
    
    /* stcc4_perform_conditioning test */
    stcc4_interface_debug_print("stcc4: stcc4_perform_conditioning test.\n");
    
    /* perform conditioning */
    res = stcc4_perform_conditioning(&gs_handle);
    if (res != 0)
    {
        stcc4_interface_debug_print("stcc4: perform conditioning failed.\n");
        (void)stcc4_deinit(&gs_handle);
        
        return 1;
    }
    stcc4_interface_debug_print("stcc4: check perform conditioning %s.\n", (res == 0) ? "ok" : "error");
    
    /* stcc4_perform_self_test test */
    stcc4_interface_debug_print("stcc4: stcc4_perform_self_test test.\n");
    
    /* perform self test */
    res = stcc4_perform_self_test(&gs_handle, &result);
    if (res != 0)
    {
        stcc4_interface_debug_print("stcc4: perform self test failed.\n");
        (void)stcc4_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    stcc4_interface_debug_print("stcc4: self test result is 0x%04X.\n", result);
    stcc4_interface_debug_print("stcc4: check perform self test %s.\n", (res == 0) ? "ok" : "error");
    
    /* stcc4_perform_forced_recalibration test */
    stcc4_interface_debug_print("stcc4: stcc4_perform_forced_recalibration test.\n");
    
    /* perform forced recalibration */
    reg = rand() % 0xFU;
    res = stcc4_perform_forced_recalibration(&gs_handle, reg, &correct_co2);
    if (res != 0)
    {
        stcc4_interface_debug_print("stcc4: perform forced recalibration failed.\n");
        (void)stcc4_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    stcc4_interface_debug_print("stcc4: correct co2 is 0x%04X.\n", correct_co2);
    stcc4_interface_debug_print("stcc4: check perform forced recalibration %s.\n", (res == 0) ? "ok" : "error");
    
    /* stcc4_perform_factory_reset test */
    stcc4_interface_debug_print("stcc4: stcc4_perform_factory_reset test.\n");
    
    /* perform factory reset */
    res = stcc4_perform_factory_reset(&gs_handle);
    if (res != 0)
    {
        stcc4_interface_debug_print("stcc4: perform factory reset failed.\n");
        (void)stcc4_deinit(&gs_handle);
        
        return 1;
    }
    stcc4_interface_debug_print("stcc4: check perform factory reset %s.\n", (res == 0) ? "ok" : "error");
    
    /* stcc4_enable_testing_mode test */
    stcc4_interface_debug_print("stcc4: stcc4_enable_testing_mode test.\n");
    
    /* enable testing mode */
    res = stcc4_enable_testing_mode(&gs_handle);
    if (res != 0)
    {
        stcc4_interface_debug_print("stcc4: enable testing mode failed.\n");
        (void)stcc4_deinit(&gs_handle);
        
        return 1;
    }
    stcc4_interface_debug_print("stcc4: check enable testing mode %s.\n", (res == 0) ? "ok" : "error");
    
    /* stcc4_disable_testing_mode test */
    stcc4_interface_debug_print("stcc4: stcc4_disable_testing_mode test.\n");
    
    /* disable testing mode */
    res = stcc4_disable_testing_mode(&gs_handle);
    if (res != 0)
    {
        stcc4_interface_debug_print("stcc4: disable testing mode failed.\n");
        (void)stcc4_deinit(&gs_handle);
        
        return 1;
    }
    stcc4_interface_debug_print("stcc4: check disable testing mode %s.\n", (res == 0) ? "ok" : "error");
    
    /* stcc4_frc_co2_convert_to_register/stcc4_frc_co2_convert_to_data test */
    stcc4_interface_debug_print("stcc4: stcc4_frc_co2_convert_to_register/stcc4_frc_co2_convert_to_data test.\n");
    
    f = (float)(rand() % 1000) / 10.0f;
    res = stcc4_frc_co2_convert_to_register(&gs_handle, f, &reg);
    if (res != 0)
    {
        stcc4_interface_debug_print("stcc4: frc co2 convert to register failed.\n");
        (void)stcc4_deinit(&gs_handle);
        
        return 1;
    }
    stcc4_interface_debug_print("stcc4: set frc co2 %.02fppm.\n", f);
    res = stcc4_frc_co2_convert_to_data(&gs_handle, reg, &f_check);
    if (res != 0)
    {
        stcc4_interface_debug_print("stcc4: frc co2 convert to data failed.\n");
        (void)stcc4_deinit(&gs_handle);
        
        return 1;
    }
    stcc4_interface_debug_print("stcc4: check frc co2 %.02fppm.\n", f_check);
    
    /* stcc4_humidity_convert_to_register/stcc4_humidity_convert_to_data test */
    stcc4_interface_debug_print("stcc4: stcc4_humidity_convert_to_register/stcc4_humidity_convert_to_data test.\n");
    
    f = (float)(rand() % 1000) / 10.0f;
    res = stcc4_humidity_convert_to_register(&gs_handle, f, &reg);
    if (res != 0)
    {
        stcc4_interface_debug_print("stcc4: humidity convert to register failed.\n");
        (void)stcc4_deinit(&gs_handle);
        
        return 1;
    }
    stcc4_interface_debug_print("stcc4: set humidity %.02f%%.\n", f);
    res = stcc4_humidity_convert_to_data(&gs_handle, reg, &f_check);
    if (res != 0)
    {
        stcc4_interface_debug_print("stcc4: humidity convert to data failed.\n");
        (void)stcc4_deinit(&gs_handle);
        
        return 1;
    }
    stcc4_interface_debug_print("stcc4: check humidity %.02f%%.\n", f_check);
    
    /* stcc4_temperature_convert_to_register/stcc4_temperature_convert_to_data test */
    stcc4_interface_debug_print("stcc4: stcc4_temperature_convert_to_register/stcc4_temperature_convert_to_data test.\n");
    
    f = (float)(rand() % 1000) / 10.0f;
    res = stcc4_temperature_convert_to_register(&gs_handle, f, &reg);
    if (res != 0)
    {
        stcc4_interface_debug_print("stcc4: temperature convert to register failed.\n");
        (void)stcc4_deinit(&gs_handle);
        
        return 1;
    }
    stcc4_interface_debug_print("stcc4: set temperature %.02fC.\n", f);
    res = stcc4_temperature_convert_to_data(&gs_handle, reg, &f_check);
    if (res != 0)
    {
        stcc4_interface_debug_print("stcc4: temperature convert to data failed.\n");
        (void)stcc4_deinit(&gs_handle);
        
        return 1;
    }
    stcc4_interface_debug_print("stcc4: check temperature %.02fC.\n", f_check);
    
    /* stcc4_pressure_convert_to_register/stcc4_pressure_convert_to_data test */
    stcc4_interface_debug_print("stcc4: stcc4_pressure_convert_to_register/stcc4_pressure_convert_to_data test.\n");
    
    f = (float)(rand() % 10000);
    res = stcc4_pressure_convert_to_register(&gs_handle, f, &reg);
    if (res != 0)
    {
        stcc4_interface_debug_print("stcc4: pressure convert to register failed.\n");
        (void)stcc4_deinit(&gs_handle);
        
        return 1;
    }
    stcc4_interface_debug_print("stcc4: set pressure %.02fpa.\n", f);
    res = stcc4_pressure_convert_to_data(&gs_handle, reg, &f_check);
    if (res != 0)
    {
        stcc4_interface_debug_print("stcc4: pressure convert to data failed.\n");
        (void)stcc4_deinit(&gs_handle);
        
        return 1;
    }
    stcc4_interface_debug_print("stcc4: check pressure %.02fpa.\n", f_check);
    
    /* stcc4_enter_sleep_mode test */
    stcc4_interface_debug_print("stcc4: stcc4_enter_sleep_mode test.\n");
    
    /* enter sleep mode */
    res = stcc4_enter_sleep_mode(&gs_handle);
    if (res != 0)
    {
        stcc4_interface_debug_print("stcc4: enter sleep mode failed.\n");
        (void)stcc4_deinit(&gs_handle);
        
        return 1;
    }
    stcc4_interface_debug_print("stcc4: check enter sleep mode %s.\n", (res == 0) ? "ok" : "error");
    
    /* stcc4_exit_sleep_mode test */
    stcc4_interface_debug_print("stcc4: stcc4_exit_sleep_mode test.\n");
    
    /* exit sleep mode */
    res = stcc4_exit_sleep_mode(&gs_handle);
    if (res != 0)
    {
        stcc4_interface_debug_print("stcc4: exit sleep mode failed.\n");
        (void)stcc4_deinit(&gs_handle);
        
        return 1;
    }
    stcc4_interface_debug_print("stcc4: check exit sleep mode %s.\n", (res == 0) ? "ok" : "error");
    
    /* stcc4_perform_soft_reset test */
    stcc4_interface_debug_print("stcc4: stcc4_perform_soft_reset test.\n");
    
    /* perform soft reset */
    res = stcc4_perform_soft_reset(&gs_handle);
    if (res != 0)
    {
        stcc4_interface_debug_print("stcc4: perform soft reset failed.\n");
        (void)stcc4_deinit(&gs_handle);
        
        return 1;
    }
    stcc4_interface_debug_print("stcc4: check perform soft reset %s.\n", (res == 0) ? "ok" : "error");
    
    /* finish register test */
    stcc4_interface_debug_print("stcc4: finish register test.\n");
    (void)stcc4_deinit(&gs_handle);
    
    return 0;
}
