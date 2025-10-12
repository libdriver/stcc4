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
 * @file      driver_stcc4_shot.h
 * @brief     driver stcc4 shot header file
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

#ifndef DRIVER_STCC4_SHOT_H
#define DRIVER_STCC4_SHOT_H

#include "driver_stcc4_interface.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @addtogroup stcc4_example_driver
 * @{
 */

/**
 * @brief     shot example init
 * @param[in] address address pin
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t stcc4_shot_init(stcc4_address_t address);

/**
 * @brief  shot example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t stcc4_shot_deinit(void);

/**
 * @brief      shot example read
 * @param[out] *co2_ppm pointer to a converted co2 buffer
 * @param[out] *temperature pointer to a converted temperature buffer
 * @param[out] *humidity pointer to a converted humidity buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 timeout
 * @note       none
 */
uint8_t stcc4_shot_read(int16_t *co2_ppm, float *temperature, float *humidity);

/**
 * @brief      shot example get product id
 * @param[out] *product_id pointer to a product id buffer
 * @param[out] *unique_serial_number pointer to a unique serial number buffer
 * @return     status code
 *             - 0 success
 *             - 1 get product id failed
 * @note       none
 */
uint8_t stcc4_shot_get_product_id(uint32_t *product_id, uint8_t unique_serial_number[8]);

/**
 * @brief  shot wake up
 * @return status code
 *         - 0 success
 *         - 1 wake up failed
 * @note   none
 */
uint8_t stcc4_shot_wake_up(void);

/**
 * @brief  shot power down
 * @return status code
 *         - 0 success
 *         - 1 power down failed
 * @note   none
 */
uint8_t stcc4_shot_power_down(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
