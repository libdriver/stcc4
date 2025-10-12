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
 * @file      driver_stcc4.h
 * @brief     driver stcc4 header file
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

#ifndef DRIVER_STCC4_H
#define DRIVER_STCC4_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup stcc4_driver stcc4 driver function
 * @brief    stcc4 driver modules
 * @{
 */

/**
 * @addtogroup stcc4_basic_driver
 * @{
 */

/**
 * @brief stcc4 address enumeration definition
 */
typedef enum
{
    STCC4_ADDRESS_0 = (0x64 << 1),        /**< addr pin connected to the GND */
    STCC4_ADDRESS_1 = (0x65 << 1),        /**< addr pin connected to the VCC */
} stcc4_address_t;

/**
 * @brief stcc4 bool enumeration definition
 */
typedef enum
{
    STCC4_BOOL_FALSE = 0x00,        /**< false */
    STCC4_BOOL_TRUE  = 0x01,        /**< true */
} stcc4_bool_t;

/**
 * @brief stcc4 handle structure definition
 */
typedef struct stcc4_handle_s
{
    uint8_t iic_addr;                                                          /**< iic address */
    uint8_t (*iic_init)(void);                                                 /**< point to an iic_init function address */
    uint8_t (*iic_deinit)(void);                                               /**< point to an iic_deinit function address */
    uint8_t (*iic_write_cmd)(uint8_t addr, uint8_t *buf, uint16_t len);        /**< point to an iic_write_cmd function address */
    uint8_t (*iic_read_cmd)(uint8_t addr, uint8_t *buf, uint16_t len);         /**< point to an iic_read_cmd function address */
    void (*delay_ms)(uint32_t ms);                                             /**< point to a delay_ms function address */
    void (*debug_print)(const char *const fmt, ...);                           /**< point to a debug_print function address */
    uint8_t inited;                                                            /**< inited flag */
    uint8_t type;                                                              /**< chip type */
} stcc4_handle_t;

/**
 * @brief stcc4 information structure definition
 */
typedef struct stcc4_info_s
{
    char chip_name[32];                /**< chip name */
    char manufacturer_name[32];        /**< manufacturer name */
    char interface[8];                 /**< chip interface name */
    float supply_voltage_min_v;        /**< chip min supply voltage */
    float supply_voltage_max_v;        /**< chip max supply voltage */
    float max_current_ma;              /**< chip max current */
    float temperature_min;             /**< chip min operating temperature */
    float temperature_max;             /**< chip max operating temperature */
    uint32_t driver_version;           /**< driver version */
} stcc4_info_t;

/**
 * @}
 */

/**
 * @defgroup stcc4_link_driver stcc4 link driver function
 * @brief    stcc4 link driver modules
 * @ingroup  stcc4_driver
 * @{
 */

/**
 * @brief     initialize stcc4_handle_t structure
 * @param[in] HANDLE pointer to a stcc4 handle structure
 * @param[in] STRUCTURE stcc4_handle_t
 * @note      none
 */
#define DRIVER_STCC4_LINK_INIT(HANDLE, STRUCTURE)            memset(HANDLE, 0, sizeof(STRUCTURE))

/**
 * @brief     link iic_init function
 * @param[in] HANDLE pointer to a stcc4 handle structure
 * @param[in] FUC pointer to an iic_init function address
 * @note      none
 */
#define DRIVER_STCC4_LINK_IIC_INIT(HANDLE, FUC)              (HANDLE)->iic_init = FUC

/**
 * @brief     link iic_deinit function
 * @param[in] HANDLE pointer to a stcc4 handle structure
 * @param[in] FUC pointer to an iic_deinit function address
 * @note      none
 */
#define DRIVER_STCC4_LINK_IIC_DEINIT(HANDLE, FUC)            (HANDLE)->iic_deinit = FUC

/**
 * @brief     link iic_read_cmd function
 * @param[in] HANDLE pointer to a stcc4 handle structure
 * @param[in] FUC pointer to an iic_read_cmd function address
 * @note      none
 */
#define DRIVER_STCC4_LINK_IIC_READ_COMMAND(HANDLE, FUC)      (HANDLE)->iic_read_cmd = FUC

/**
 * @brief     link iic_write_cmd function
 * @param[in] HANDLE pointer to a stcc4 handle structure
 * @param[in] FUC pointer to an iic_write_cmd function address
 * @note      none
 */
#define DRIVER_STCC4_LINK_IIC_WRITE_COMMAND(HANDLE, FUC)     (HANDLE)->iic_write_cmd = FUC

/**
 * @brief     link delay_ms function
 * @param[in] HANDLE pointer to a stcc4 handle structure
 * @param[in] FUC pointer to a delay_ms function address
 * @note      none
 */
#define DRIVER_STCC4_LINK_DELAY_MS(HANDLE, FUC)              (HANDLE)->delay_ms = FUC

/**
 * @brief     link debug_print function
 * @param[in] HANDLE pointer to a stcc4 handle structure
 * @param[in] FUC pointer to a debug_print function address
 * @note      none
 */
#define DRIVER_STCC4_LINK_DEBUG_PRINT(HANDLE, FUC)           (HANDLE)->debug_print = FUC

/**
 * @}
 */

/**
 * @defgroup stcc4_basic_driver stcc4 basic driver function
 * @brief    stcc4 basic driver modules
 * @ingroup  stcc4_driver
 * @{
 */

/**
 * @brief      get chip information
 * @param[out] *info pointer to an stcc4 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t stcc4_info(stcc4_info_t *info);

/**
 * @brief     set address pin
 * @param[in] *handle pointer to a stcc4 handle structure
 * @param[in] address address pin
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t stcc4_set_address_pin(stcc4_handle_t *handle, stcc4_address_t address);

/**
 * @brief      get address pin
 * @param[in]  *handle pointer to a stcc4 handle structure
 * @param[out] *address pointer to an address buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t stcc4_get_address_pin(stcc4_handle_t *handle, stcc4_address_t *address);

/**
 * @brief     initialize the chip
 * @param[in] *handle pointer to an stcc4 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic initialization failed
 *            - 2 handle is NULL
 *            - 3 linked functions is NULL
 *            - 4 product id is invalid
 * @note      none
 */
uint8_t stcc4_init(stcc4_handle_t *handle);

/**
 * @brief     close the chip
 * @param[in] *handle pointer to an stcc4 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic deinit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 stop continuous measurement failed
 * @note      none
 */
uint8_t stcc4_deinit(stcc4_handle_t *handle);

/**
 * @brief      read data
 * @param[in]  *handle pointer to a stcc4 handle structure
 * @param[out] *co2_raw pointer to a co2 raw buffer
 * @param[out] *co2_ppm pointer to a co2 ppm buffer
 * @param[out] *temperature_raw pointer to a temperature raw buffer
 * @param[out] *temperature_s pointer to a temperature buffer
 * @param[out] *humidity_raw pointer to a humidity raw buffer
 * @param[out] *humidity_s pointer to a humidity buffer
 * @param[out] *sensor_status pointer to a sensor status buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 crc is error
 *             - 5 sensor status is invalid
 * @note       none
 */
uint8_t stcc4_read(stcc4_handle_t *handle, int16_t *co2_raw, int16_t *co2_ppm,
                   uint16_t *temperature_raw, float *temperature_s,
                   uint16_t *humidity_raw, float *humidity_s, uint16_t *sensor_status);

/**
 * @brief     start continuous measurement
 * @param[in] *handle pointer to an stcc4 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 start continuous measurement failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t stcc4_start_continuous_measurement(stcc4_handle_t *handle);

/**
 * @brief     stop continuous measurement
 * @param[in] *handle pointer to an stcc4 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 stop continuous measurement failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t stcc4_stop_continuous_measurement(stcc4_handle_t *handle);

/**
 * @brief     set rht compensation
 * @param[in] *handle pointer to a stcc4 handle structure
 * @param[in] temperature_raw raw temperature
 * @param[in] humidity_raw raw humidity
 * @return    status code
 *            - 0 success
 *            - 1 set rht compensation failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t stcc4_set_rht_compensation(stcc4_handle_t *handle, uint16_t temperature_raw, uint16_t humidity_raw);

/**
 * @brief     set pressure compensation
 * @param[in] *handle pointer to a stcc4 handle structure
 * @param[in] pressure_raw raw pressure
 * @return    status code
 *            - 0 success
 *            - 1 set pressure compensation failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t stcc4_set_pressure_compensation(stcc4_handle_t *handle, uint16_t pressure_raw);

/**
 * @brief     measure single shot
 * @param[in] *handle pointer to an stcc4 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 measure single shot failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t stcc4_measure_single_shot(stcc4_handle_t *handle);

/**
 * @brief     enter sleep mode
 * @param[in] *handle pointer to an stcc4 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 enter sleep mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t stcc4_enter_sleep_mode(stcc4_handle_t *handle);

/**
 * @brief     exit sleep mode
 * @param[in] *handle pointer to an stcc4 handle structure
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t stcc4_exit_sleep_mode(stcc4_handle_t *handle);

/**
 * @brief     perform conditioning
 * @param[in] *handle pointer to an stcc4 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 perform conditioning failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t stcc4_perform_conditioning(stcc4_handle_t *handle);

/**
 * @brief     perform soft reset
 * @param[in] *handle pointer to an stcc4 handle structure
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t stcc4_perform_soft_reset(stcc4_handle_t *handle);

/**
 * @brief     perform factory reset
 * @param[in] *handle pointer to an stcc4 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 perform factory reset failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 not passed
 * @note      none
 */
uint8_t stcc4_perform_factory_reset(stcc4_handle_t *handle);

/**
 * @brief      perform self test
 * @param[in]  *handle pointer to a stcc4 handle structure
 * @param[out] *result pointer to a result buffer
 * @return     status code
 *             - 0 success
 *             - 1 perform self test failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 crc check failed
 * @note       none
 */
uint8_t stcc4_perform_self_test(stcc4_handle_t *handle, uint16_t *result);

/**
 * @brief     enable testing mode
 * @param[in] *handle pointer to an stcc4 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 enable testing mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t stcc4_enable_testing_mode(stcc4_handle_t *handle);

/**
 * @brief     disable testing mode
 * @param[in] *handle pointer to an stcc4 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 disable testing mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t stcc4_disable_testing_mode(stcc4_handle_t *handle);

/**
 * @brief      perform forced recalibration
 * @param[in]  *handle pointer to a stcc4 handle structure
 * @param[in]  target_co2 input target co2
 * @param[out] *correct_co2 pointer to a correct co2 buffer
 * @return     status code
 *             - 0 success
 *             - 1 perform forced recalibration failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 crc check failed
 * @note       none
 */
uint8_t stcc4_perform_forced_recalibration(stcc4_handle_t *handle, uint16_t target_co2, uint16_t *correct_co2);

/**
 * @brief      get product id
 * @param[in]  *handle pointer to a stcc4 handle structure
 * @param[out] *product_id pointer to a product id buffer
 * @param[out] *unique_serial_number pointer to a unique serial number buffer
 * @return     status code
 *             - 0 success
 *             - 1 get product id failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 crc check failed
 * @note       none
 */
uint8_t stcc4_get_product_id(stcc4_handle_t *handle, uint32_t *product_id, uint8_t unique_serial_number[8]);

/**
 * @brief      convert the frc co2 to the register raw data
 * @param[in]  *handle pointer to a stcc4 handle structure
 * @param[in]  ppm co2 ppm
 * @param[out] *reg pointer to a register raw buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t stcc4_frc_co2_convert_to_register(stcc4_handle_t *handle, float ppm, uint16_t *reg);

/**
 * @brief      convert the register raw data to frc co2 ppm
 * @param[in]  *handle pointer to a stcc4 handle structure
 * @param[in]  reg register raw data
 * @param[out] *ppm pointer to a ppm buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t stcc4_frc_co2_convert_to_data(stcc4_handle_t *handle, uint16_t reg, float *ppm);

/**
 * @brief      convert the humidity to the register raw data
 * @param[in]  *handle pointer to a stcc4 handle structure
 * @param[in]  percentage humidity percentage
 * @param[out] *reg pointer to a register raw buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t stcc4_humidity_convert_to_register(stcc4_handle_t *handle, float percentage, uint16_t *reg);

/**
 * @brief      convert the register raw data to humidity
 * @param[in]  *handle pointer to a stcc4 handle structure
 * @param[in]  reg register raw data
 * @param[out] *percentage pointer to a percentage buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t stcc4_humidity_convert_to_data(stcc4_handle_t *handle, uint16_t reg, float *percentage);

/**
 * @brief      convert the temperature to the register raw data
 * @param[in]  *handle pointer to a stcc4 handle structure
 * @param[in]  deg celsius degrees
 * @param[out] *reg pointer to a register raw buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t stcc4_temperature_convert_to_register(stcc4_handle_t *handle, float deg, uint16_t *reg);

/**
 * @brief      convert the register raw data to temperature
 * @param[in]  *handle pointer to a stcc4 handle structure
 * @param[in]  reg register raw data
 * @param[out] *deg pointer to a degrees buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t stcc4_temperature_convert_to_data(stcc4_handle_t *handle, uint16_t reg, float *deg);

/**
 * @brief      convert the pressure to the register raw data
 * @param[in]  *handle pointer to a stcc4 handle structure
 * @param[in]  pa pressure pascal
 * @param[out] *reg pointer to a register raw buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t stcc4_pressure_convert_to_register(stcc4_handle_t *handle, float pa, uint16_t *reg);

/**
 * @brief      convert the register raw data to pressure
 * @param[in]  *handle pointer to a stcc4 handle structure
 * @param[in]  reg register raw data
 * @param[out] *pa pointer to a pressure buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t stcc4_pressure_convert_to_data(stcc4_handle_t *handle, uint16_t reg, float *pa);

/**
 * @}
 */

/**
 * @defgroup stcc4_extern_driver stcc4 extern driver function
 * @brief    stcc4 extern driver modules
 * @ingroup  stcc4_driver
 * @{
 */

/**
 * @brief     set the chip register
 * @param[in] *handle pointer to a stcc4 handle structure
 * @param[in] reg iic register address
 * @param[in] *buf pointer to a data buffer
 * @param[in] len data buffer length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t stcc4_set_reg(stcc4_handle_t *handle, uint16_t reg, uint8_t *buf, uint16_t len);

/**
 * @brief      get the chip register
 * @param[in]  *handle pointer to a stcc4 handle structure
 * @param[in]  reg iic register address
 * @param[out] *buf pointer to a data buffer
 * @param[in]  len data buffer length
 * @param[in]  delay_ms delay time in ms
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t stcc4_get_reg(stcc4_handle_t *handle, uint16_t reg, uint8_t *buf, uint16_t len, uint16_t delay_ms);

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
