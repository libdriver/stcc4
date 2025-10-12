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
 * @file      driver_stcc4_basic.c
 * @brief     driver stcc4 basic source file
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

#include "driver_stcc4_basic.h"

static stcc4_handle_t gs_handle;        /**< stcc4 handle */

/**
 * @brief     basic example init
 * @param[in] address address pin
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t stcc4_basic_init(stcc4_address_t address)
{
    uint8_t res;
    
    /* link functions */
    DRIVER_STCC4_LINK_INIT(&gs_handle, stcc4_handle_t);
    DRIVER_STCC4_LINK_IIC_INIT(&gs_handle, stcc4_interface_iic_init);
    DRIVER_STCC4_LINK_IIC_DEINIT(&gs_handle, stcc4_interface_iic_deinit);
    DRIVER_STCC4_LINK_IIC_WRITE_COMMAND(&gs_handle, stcc4_interface_iic_write_cmd);
    DRIVER_STCC4_LINK_IIC_READ_COMMAND(&gs_handle, stcc4_interface_iic_read_cmd);
    DRIVER_STCC4_LINK_DELAY_MS(&gs_handle, stcc4_interface_delay_ms);
    DRIVER_STCC4_LINK_DEBUG_PRINT(&gs_handle, stcc4_interface_debug_print);

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
    
    /* start continuous measurement */
    res = stcc4_start_continuous_measurement(&gs_handle);
    if (res != 0)
    {
        stcc4_interface_debug_print("stcc4: start continuous measurement failed.\n");
        (void)stcc4_deinit(&gs_handle);
        
        return 1;
    }
    
    return 0;
}

/**
 * @brief      basic example read
 * @param[out] *co2_ppm pointer to a converted co2 buffer
 * @param[out] *temperature pointer to a converted temperature buffer
 * @param[out] *humidity pointer to a converted humidity buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t stcc4_basic_read(int16_t *co2_ppm, float *temperature, float *humidity)
{
    uint8_t res;
    int16_t co2_raw;
    uint16_t temperature_raw;
    uint16_t humidity_raw;
    uint16_t sensor_status;
    
    /* read data */
    res = stcc4_read(&gs_handle, &co2_raw, co2_ppm,
                     &temperature_raw, temperature,
                     &humidity_raw, humidity, &sensor_status);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief  basic example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t stcc4_basic_deinit(void)
{
    /* stop */
    if (stcc4_stop_continuous_measurement(&gs_handle) != 0)
    {
        return 1;
    }
    
    /* close stcc4 */
    if (stcc4_deinit(&gs_handle) != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief      basic example get product id
 * @param[out] *product_id pointer to a product id buffer
 * @param[out] *unique_serial_number pointer to a unique serial number buffer
 * @return     status code
 *             - 0 success
 *             - 1 get product id failed
 * @note       none
 */
uint8_t stcc4_basic_get_product_id(uint32_t *product_id, uint8_t unique_serial_number[8])
{
    /* get product id */
    if (stcc4_get_product_id(&gs_handle, product_id, unique_serial_number) != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief  basic wake up
 * @return status code
 *         - 0 success
 *         - 1 wake up failed
 * @note   none
 */
uint8_t stcc4_basic_wake_up(void)
{
    /* wake up */
    if (stcc4_exit_sleep_mode(&gs_handle) != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief  basic power down
 * @return status code
 *         - 0 success
 *         - 1 power down failed
 * @note   none
 */
uint8_t stcc4_basic_power_down(void)
{
    /* power down */
    if (stcc4_enter_sleep_mode(&gs_handle) != 0)
    {
        return 1;
    }
    
    return 0;
}
