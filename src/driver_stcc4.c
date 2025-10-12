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
 * @file      driver_stcc4.c
 * @brief     driver stcc4 source file
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

#include "driver_stcc4.h"

/**
 * @brief chip information definition
 */
#define CHIP_NAME                 "Sensirion STCC4"        /**< chip name */
#define MANUFACTURER_NAME         "Sensirion"              /**< manufacturer name */
#define SUPPLY_VOLTAGE_MIN        2.7f                     /**< chip min supply voltage */
#define SUPPLY_VOLTAGE_MAX        5.5f                     /**< chip max supply voltage */
#define MAX_CURRENT               4.2f                     /**< chip max current */
#define TEMPERATURE_MIN           -40.0f                   /**< chip min operating temperature */
#define TEMPERATURE_MAX           85.0f                    /**< chip max operating temperature */
#define DRIVER_VERSION            1000                     /**< driver version */

/**
 * @brief chip command definition
 */
#define STCC4_COMMAND_START_CONTINUOUS_MEASUREMENT     0x218BU        /**< start continuous measurement command */
#define STCC4_COMMAND_STOP_CONTINUOUS_MEASUREMENT      0x3F86U        /**< stop continuous measurement command */
#define STCC4_COMMAND_READ_MEASUREMENT                 0xEC05U        /**< read measurement command */
#define STCC4_COMMAND_SET_RHT_COMPENSATION             0xE000U        /**< set rht compensation command */
#define STCC4_COMMAND_SET_PRESSURER_COMPENSATION       0xE016U        /**< set pressure compensation command */
#define STCC4_COMMAND_MEASURE_SINGLE_SHOT              0x219DU        /**< measure single shot command */
#define STCC4_COMMAND_ENTER_SLEEP_MODE                 0x3650U        /**< enter sleep mode command */
#define STCC4_COMMAND_EXIT_SLEEP_MODE                  0x00U          /**< exit sleep mode command */
#define STCC4_COMMAND_PERFORM_CONDITIONING             0x29BCU        /**< perform conditioning command */
#define STCC4_COMMAND_PERFORM_SOFT_RESET               0x06U          /**< perform soft reset command */
#define STCC4_COMMAND_PERFORM_FACTORY_RESET            0x3632U        /**< perform factory reset command */
#define STCC4_COMMAND_PERFORM_SELF_TEST                0x278CU        /**< perform self test command */
#define STCC4_COMMAND_ENABLE_TESTING_MODE              0x3FBCU        /**< enable testing mode command */
#define STCC4_COMMAND_DISABLE_TESTING_MODE             0x3F3DU        /**< disable testing mode command */
#define STCC4_COMMAND_PERFORM_FORCED_RECALIBRATION     0x362FU        /**< perform forced recalibration command */
#define STCC4_COMMAND_GET_PRODUCT_ID                   0x365BU        /**< get product id command */

/**
 * @brief crc8 definition
 */
#define STCC4_CRC8_POLYNOMIAL        0x31
#define STCC4_CRC8_INIT              0xFF

/**
 * @brief      read bytes with param
 * @param[in]  *handle pointer to a stcc4 handle structure
 * @param[in]  reg iic register address
 * @param[in]  *data pointer to a data buffer
 * @param[in]  len data length
 * @param[in]  delay_ms delay time in ms
 * @param[out] *output pointer to an output buffer
 * @param[in]  output_len output length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
static uint8_t a_stcc4_iic_read_with_param(stcc4_handle_t *handle, uint16_t reg, uint8_t *data, uint16_t len,
                                           uint16_t delay_ms, uint8_t *output, uint16_t output_len)
{
    uint8_t buf[16];
    uint16_t i;
        
    if ((len + 2) > 16)                                                            /* check length */
    {
        return 1;                                                                  /* return error */
    }
    memset(buf, 0, sizeof(uint8_t) * 16);                                          /* clear the buffer */
    buf[0] = (uint8_t)((reg >> 8) & 0xFF);                                         /* set MSB of reg */
    buf[1] = (uint8_t)(reg & 0xFF);                                                /* set LSB of reg */
    for (i = 0; i < len; i++)
    {
        buf[2 + i] = data[i];                                                      /* copy write data */
    }
    
    if (handle->iic_write_cmd(handle->iic_addr, (uint8_t *)buf, len + 2) != 0)     /* write iic command */
    {
        return 1;                                                                  /* write command */
    }
    handle->delay_ms(delay_ms);                                                    /* delay ms */
    if (handle->iic_read_cmd(handle->iic_addr, output, output_len) != 0)           /* read data */
    {
        return 1;                                                                  /* write command */
    }
    else
    {
        return 0;                                                                  /* success return 0 */
    }
}

/**
 * @brief      read bytes
 * @param[in]  *handle pointer to a stcc4 handle structure
 * @param[in]  reg iic register address
 * @param[out] *data pointer to a data buffer
 * @param[in]  len data length
 * @param[in]  delay_ms delay time in ms
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
static uint8_t a_stcc4_iic_read(stcc4_handle_t *handle, uint16_t reg, uint8_t *data, uint16_t len, uint16_t delay_ms)
{
    uint8_t buf[2];
    
    memset(buf, 0, sizeof(uint8_t) * 2);                                     /* clear the buffer */
    buf[0] = (uint8_t)((reg >> 8) & 0xFF);                                   /* set reg MSB */
    buf[1] = (uint8_t)(reg & 0xFF);                                          /* set reg LSB */
    if (handle->iic_write_cmd(handle->iic_addr, (uint8_t *)buf, 2) != 0)     /* write command */
    {   
        return 1;                                                            /* return error */
    }
    handle->delay_ms(delay_ms);                                              /* delay ms */
    if (handle->iic_read_cmd(handle->iic_addr, data, len) != 0)              /* read data */
    {
        return 1;                                                            /* write command */
    }
    else
    {
        return 0;                                                            /* success return 0 */
    }
}

/**
 * @brief     write bytes
 * @param[in] *handle pointer to a stcc4 handle structure
 * @param[in] reg iic register address
 * @param[in] *data pointer to a data buffer
 * @param[in] len data length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
static uint8_t a_stcc4_iic_write(stcc4_handle_t *handle, uint16_t reg, uint8_t *data, uint16_t len)
{
    uint8_t buf[16];
    uint16_t i;
        
    if ((len + 2) > 16)                                                            /* check length */
    {
        return 1;                                                                  /* return error */
    }
    memset(buf, 0, sizeof(uint8_t) * 16);                                          /* clear the buffer */
    buf[0] = (uint8_t)((reg >> 8) & 0xFF);                                         /* set MSB of reg */
    buf[1] = (uint8_t)(reg & 0xFF);                                                /* set LSB of reg */
    for (i = 0; i < len; i++)
    {
        buf[2 + i] = data[i];                                                      /* copy write data */
    }
    
    if (handle->iic_write_cmd(handle->iic_addr, (uint8_t *)buf, len + 2) != 0)     /* write iic command */
    {
        return 1;                                                                  /* write command */
    }
    else
    {
        return 0;                                                                  /* success return 0 */
    }
}

/**
 * @brief     generate the crc
 * @param[in] *data pointer to a data buffer
 * @param[in] count data length
 * @return    crc
 * @note      none
 */
static uint8_t a_stcc4_generate_crc(uint8_t* data, uint8_t count)
{
    uint8_t current_byte;
    uint8_t crc = STCC4_CRC8_INIT;
    uint8_t crc_bit;

    for (current_byte = 0; current_byte < count; ++current_byte)        /* calculate crc */
    {
        crc ^= (data[current_byte]);                                    /* xor data */
        for (crc_bit = 8; crc_bit > 0; --crc_bit)                       /* 8 bit */
        {
            if ((crc & 0x80) != 0)                                      /* if 7th bit is 1 */
            {
                crc = (crc << 1) ^ STCC4_CRC8_POLYNOMIAL;               /* xor */
            }
            else
            {
                crc = crc << 1;                                         /* left shift 1 */
            }
        }
    }
    
    return crc;                                                         /* return crc */
}

/**
 * @brief     set address pin
 * @param[in] *handle pointer to a stcc4 handle structure
 * @param[in] address address pin
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t stcc4_set_address_pin(stcc4_handle_t *handle, stcc4_address_t address)
{
    if (handle == NULL)                         /* check handle */
    {
        return 2;                               /* return error */
    }
    
    handle->iic_addr = (uint8_t)address;        /* set address */
    
    return 0;                                   /* success return 0 */
}

/**
 * @brief      get address pin
 * @param[in]  *handle pointer to a stcc4 handle structure
 * @param[out] *address pointer to an address buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t stcc4_get_address_pin(stcc4_handle_t *handle, stcc4_address_t *address)
{
    if (handle == NULL)                                    /* check handle */
    {
        return 2;                                          /* return error */
    }
    
    *address = (stcc4_address_t)(handle->iic_addr);        /* get address */
    
    return 0;                                              /* success return 0 */
}

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
uint8_t stcc4_start_continuous_measurement(stcc4_handle_t *handle)
{
    uint8_t res;
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }

    res = a_stcc4_iic_write(handle, STCC4_COMMAND_START_CONTINUOUS_MEASUREMENT, NULL, 0);        /* write config */
    if (res != 0)                                                                                /* check result */
    {
        handle->debug_print("stcc4: start continuous measurement failed.\n");                    /* start continuous measurement failed */
       
        return 1;                                                                                /* return error */
    }
    handle->delay_ms(1000);                                                                      /* wait 1000ms */
    
    return 0;                                                                                    /* success return 0 */
}

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
uint8_t stcc4_stop_continuous_measurement(stcc4_handle_t *handle)
{
    uint8_t res;
    
    if (handle == NULL)                                                                         /* check handle */
    {
        return 2;                                                                               /* return error */
    }
    if (handle->inited != 1)                                                                    /* check handle initialization */
    {
        return 3;                                                                               /* return error */
    }

    res = a_stcc4_iic_write(handle, STCC4_COMMAND_STOP_CONTINUOUS_MEASUREMENT, NULL, 0);        /* write config */
    if (res != 0)                                                                               /* check result */
    {
        handle->debug_print("stcc4: stop continuous measurement failed.\n");                    /* stop continuous measurement failed */
       
        return 1;                                                                               /* return error */
    }
    handle->delay_ms(1200);                                                                     /* wait 1200ms */
    
    return 0;                                                                                   /* success return 0 */
}

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
uint8_t stcc4_set_rht_compensation(stcc4_handle_t *handle, uint16_t temperature_raw, uint16_t humidity_raw)
{
    uint8_t res;
    uint8_t buf[6];
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }

    buf[0] = (temperature_raw >> 8) & 0xFF;                                             /* set temperature msb */
    buf[1] = (temperature_raw >> 0) & 0xFF;                                             /* set temperature lsb */
    buf[2] = a_stcc4_generate_crc(buf + 0, 2);                                          /* set temperature crc */
    buf[3] = (humidity_raw >> 8) & 0xFF;                                                /* set humidity msb */
    buf[4] = (humidity_raw >> 0) & 0xFF;                                                /* set humidity lsb */
    buf[5] = a_stcc4_generate_crc(buf + 3, 2);                                          /* set humidity crc */
    res = a_stcc4_iic_write(handle, STCC4_COMMAND_SET_RHT_COMPENSATION, buf, 6);        /* write config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("stcc4: set rht compensation failed.\n");                   /* set rht compensation failed */
       
        return 1;                                                                       /* return error */
    }
    handle->delay_ms(1);                                                                /* wait 1ms */
    
    return 0;                                                                           /* success return 0 */
}

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
uint8_t stcc4_set_pressure_compensation(stcc4_handle_t *handle, uint16_t pressure_raw)
{
    uint8_t res;
    uint8_t buf[3];
    
    if (handle == NULL)                                                                       /* check handle */
    {
        return 2;                                                                             /* return error */
    }
    if (handle->inited != 1)                                                                  /* check handle initialization */
    {
        return 3;                                                                             /* return error */
    }

    buf[0] = (pressure_raw >> 8) & 0xFF;                                                      /* set pressure msb */
    buf[1] = (pressure_raw >> 0) & 0xFF;                                                      /* set pressure lsb */
    buf[2] = a_stcc4_generate_crc(buf + 0, 2);                                                /* set pressure crc */
    res = a_stcc4_iic_write(handle, STCC4_COMMAND_SET_PRESSURER_COMPENSATION, buf, 3);        /* write config */
    if (res != 0)                                                                             /* check result */
    {
        handle->debug_print("stcc4: set pressure compensation failed.\n");                    /* set pressure compensation failed */
       
        return 1;                                                                             /* return error */
    }
    handle->delay_ms(1);                                                                      /* wait 1ms */
    
    return 0;                                                                                 /* success return 0 */
}

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
uint8_t stcc4_measure_single_shot(stcc4_handle_t *handle)
{
    uint8_t res;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }

    res = a_stcc4_iic_write(handle, STCC4_COMMAND_MEASURE_SINGLE_SHOT, NULL, 0);        /* write config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("stcc4: measure single shot failed.\n");                    /* measure single shot failed */
       
        return 1;                                                                       /* return error */
    }
    handle->delay_ms(500);                                                              /* wait 500ms */
    
    return 0;                                                                           /* success return 0 */
}

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
uint8_t stcc4_enter_sleep_mode(stcc4_handle_t *handle)
{
    uint8_t res;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }

    res = a_stcc4_iic_write(handle, STCC4_COMMAND_ENTER_SLEEP_MODE, NULL, 0);        /* write config */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("stcc4: enter sleep mode failed.\n");                    /* enter sleep mode failed */
       
        return 1;                                                                    /* return error */
    }
    handle->delay_ms(1);                                                             /* wait 1ms */
    
    return 0;                                                                        /* success return 0 */
}

/**
 * @brief     exit sleep mode
 * @param[in] *handle pointer to an stcc4 handle structure
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t stcc4_exit_sleep_mode(stcc4_handle_t *handle)
{
    uint8_t reg;
    
    if (handle == NULL)                                            /* check handle */
    {
        return 2;                                                  /* return error */
    }
    if (handle->inited != 1)                                       /* check handle initialization */
    {
        return 3;                                                  /* return error */
    }

    reg = STCC4_COMMAND_EXIT_SLEEP_MODE;                           /* set exit sleep command */
    (void)handle->iic_write_cmd(handle->iic_addr, &reg, 1);        /* exit sleep mode */
    handle->delay_ms(5);                                           /* wait 5ms */
    
    return 0;                                                      /* success return 0 */
}

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
uint8_t stcc4_perform_conditioning(stcc4_handle_t *handle)
{
    uint8_t res;
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }

    res = a_stcc4_iic_write(handle, STCC4_COMMAND_PERFORM_CONDITIONING, NULL, 0);        /* write config */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("stcc4: perform conditioning failed.\n");                    /* perform conditioning failed */
       
        return 1;                                                                        /* return error */
    }
    handle->delay_ms(22000);                                                             /* wait 22000ms */
    
    return 0;                                                                            /* success return 0 */
}

/**
 * @brief     perform soft reset
 * @param[in] *handle pointer to an stcc4 handle structure
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t stcc4_perform_soft_reset(stcc4_handle_t *handle)
{
    uint8_t reg;
    
    if (handle == NULL)                                /* check handle */
    {
        return 2;                                      /* return error */
    }
    if (handle->inited != 1)                           /* check handle initialization */
    {
        return 3;                                      /* return error */
    }

    reg = STCC4_COMMAND_PERFORM_SOFT_RESET;            /* perform soft reset command */
    (void)handle->iic_write_cmd(0x00, &reg, 1);        /* perform soft reset */
    handle->delay_ms(10);                              /* wait 10ms */
    
    return 0;                                          /* success return 0 */
}

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
uint8_t stcc4_perform_factory_reset(stcc4_handle_t *handle)
{
    uint8_t res;
    uint8_t buf[2];
    uint16_t result;
    
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }

    res = a_stcc4_iic_read(handle, STCC4_COMMAND_PERFORM_FACTORY_RESET, buf, 2, 90);        /* write config */
    if (res != 0)                                                                           /* check result */
    {
        handle->debug_print("stcc4: perform factory reset failed.\n");                      /* perform factory reset failed */
       
        return 1;                                                                           /* return error */
    }
    result = (uint16_t)((uint16_t)buf[0] << 8) | buf[1];                                    /* set the result */
    if (result != 0)                                                                        /* check the result */
    {
        handle->debug_print("stcc4: not passed.\n");                                        /* not passed */
       
        return 4;                                                                           /* return error */
    }
    
    return 0;                                                                               /* success return 0 */
}

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
uint8_t stcc4_perform_self_test(stcc4_handle_t *handle, uint16_t *result)
{
    uint8_t res;
    uint8_t buf[3];
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }

    res = a_stcc4_iic_read(handle, STCC4_COMMAND_PERFORM_SELF_TEST, buf, 3, 360);        /* write config */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("stcc4: perform self test failed.\n");                       /* perform self test failed */
       
        return 1;                                                                        /* return error */
    }
    if (a_stcc4_generate_crc(buf, 2) != buf[2])                                          /* check crc */
    {
        handle->debug_print("stcc4: crc check failed.\n");                               /* crc check failed */
       
        return 4;                                                                        /* return error */
    }
    *result = (uint16_t)((uint16_t)buf[0] << 8) | buf[1];                                /* set the result */
    
    return 0;                                                                            /* success return 0 */
}

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
uint8_t stcc4_enable_testing_mode(stcc4_handle_t *handle)
{
    uint8_t res;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }

    res = a_stcc4_iic_write(handle, STCC4_COMMAND_ENABLE_TESTING_MODE, NULL, 0);        /* write config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("stcc4: enable testing mode failed.\n");                    /* enable testing mode failed */
       
        return 1;                                                                       /* return error */
    }
    
    return 0;                                                                           /* success return 0 */
}

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
uint8_t stcc4_disable_testing_mode(stcc4_handle_t *handle)
{
    uint8_t res;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }

    res = a_stcc4_iic_write(handle, STCC4_COMMAND_DISABLE_TESTING_MODE, NULL, 0);       /* write config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("stcc4: disable testing mode failed.\n");                   /* disable testing mode failed */
       
        return 1;                                                                       /* return error */
    }
    
    return 0;                                                                           /* success return 0 */
}

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
uint8_t stcc4_perform_forced_recalibration(stcc4_handle_t *handle, uint16_t target_co2, uint16_t *correct_co2)
{
    uint8_t res;
    uint8_t buf[3];
    uint8_t output[3];
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    
    buf[0] = (target_co2 >> 8) & 0xFF;                                                           /* set target co2 msb */
    buf[1] = (target_co2 >> 0) & 0xFF;                                                           /* set target co2 lsb */
    buf[2] = a_stcc4_generate_crc(buf, 2);                                                       /* set target co2 crc */
    res = a_stcc4_iic_read_with_param(handle, STCC4_COMMAND_PERFORM_FORCED_RECALIBRATION, 
                                      buf, 3, 90, output, 3);                                    /* write config */
    if (res != 0)                                                                                /* check result */
    {
        handle->debug_print("stcc4: perform forced recalibration failed.\n");                    /* perform forced recalibration failed */
       
        return 1;                                                                                /* return error */
    }
    if (a_stcc4_generate_crc(output, 2) != output[2])                                            /* check crc */
    {
        handle->debug_print("stcc4: crc check failed.\n");                                       /* crc check failed */
       
        return 4;                                                                                /* return error */
    }
    *correct_co2 = (uint16_t)((uint16_t)output[0] << 8) | output[1];                             /* set the correct co2 */
    
    return 0;                                                                                    /* success return 0 */
}

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
uint8_t stcc4_get_product_id(stcc4_handle_t *handle, uint32_t *product_id, uint8_t unique_serial_number[8])
{
    uint8_t res;
    uint8_t i;
    uint8_t buf[18];
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }

    res = a_stcc4_iic_read(handle, STCC4_COMMAND_GET_PRODUCT_ID, buf, 18, 1);        /* write config */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("stcc4: get product id failed.\n");                      /* get product id failed */
       
        return 1;                                                                    /* return error */
    }
    for (i = 0; i < 18; i += 3)                                                      /* check all */
    {
        if (a_stcc4_generate_crc(buf + i, 2) != buf[i + 2])                          /* check crc */
        {
            handle->debug_print("stcc4: crc check failed.\n");                       /* crc check failed */
           
            return 4;                                                                /* return error */
        }
    }
    *product_id = ((uint32_t)buf[0] << 24) | ((uint32_t)buf[1] << 16) |
                  ((uint32_t)buf[3] << 8) | ((uint32_t)buf[4] << 0);                 /* set product id */
    unique_serial_number[0] = buf[6];                                                /* set unique serial number index 0 */
    unique_serial_number[1] = buf[7];                                                /* set unique serial number index 1 */
    unique_serial_number[2] = buf[9];                                                /* set unique serial number index 2 */
    unique_serial_number[3] = buf[10];                                               /* set unique serial number index 3 */
    unique_serial_number[4] = buf[12];                                               /* set unique serial number index 4 */
    unique_serial_number[5] = buf[13];                                               /* set unique serial number index 5 */
    unique_serial_number[6] = buf[15];                                               /* set unique serial number index 6 */
    unique_serial_number[7] = buf[16];                                               /* set unique serial number index 7 */
    
    return 0;                                                                        /* success return 0 */
}

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
                   uint16_t *humidity_raw, float *humidity_s, uint16_t *sensor_status)
{
    uint8_t res;
    uint8_t buf[12];
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_stcc4_iic_read(handle, STCC4_COMMAND_READ_MEASUREMENT, buf, 12, 1);           /* read data */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("stcc4: read measurement failed.\n");                         /* read measurement failed */
       
        return 1;                                                                         /* return error */
    }
    
    if (buf[2] != a_stcc4_generate_crc(&buf[0], 2))                                       /* check crc */
    {
        handle->debug_print("stcc4: crc is error.\n");                                    /* crc is error */
       
        return 4;                                                                         /* return error */
    }
    if (buf[5] != a_stcc4_generate_crc(&buf[3], 2))                                       /* check crc */
    {
        handle->debug_print("stcc4: crc is error.\n");                                    /* crc is error */
       
        return 4;                                                                         /* return error */
    }
    if (buf[8] != a_stcc4_generate_crc(&buf[6], 2))                                       /* check crc */
    {
        handle->debug_print("stcc4: crc is error.\n");                                    /* crc is error */
       
        return 4;                                                                         /* return error */
    }
    if (buf[11] != a_stcc4_generate_crc(&buf[9], 2))                                      /* check crc */
    {
        handle->debug_print("stcc4: crc is error.\n");                                    /* crc is error */
       
        return 4;                                                                         /* return error */
    }
    
    *co2_raw = (int16_t)(((uint16_t)buf[0]) << 8) | buf[1];                               /* set co2 raw */
    *temperature_raw = (uint16_t)(((uint16_t)buf[3]) << 8) | buf[4];                      /* set temperature raw */
    *humidity_raw = (uint16_t)(((uint16_t)buf[6]) << 8) | buf[7];                         /* set humidity raw */
    *sensor_status = (uint16_t)(((uint16_t)buf[9]) << 8) | buf[10];                       /* set sensor status */
    *co2_ppm = *co2_raw;                                                                  /* set co2 ppm */
    *temperature_s = -45.0f + 175.0f * (float)(*temperature_raw) / 65535.0f;              /* set temperature */
    *humidity_s = 125.0f * (float)(*humidity_raw) / 65535.0f - 6.0f;                      /* set humidity */
    if ((*sensor_status) != 0)                                                            /* check sensor status */
    {
        handle->debug_print("stcc4: sensor status is invalid.\n");                        /* sensor status is invalid */
       
        return 5;                                                                         /* return error */
    }
    
    return 0;                                                                             /* success return 0 */
}

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
uint8_t stcc4_init(stcc4_handle_t *handle)
{
    uint8_t res;
    uint8_t reg;
    uint8_t i;
    uint32_t product_id;
    uint8_t buf[18];
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->debug_print == NULL)                                                 /* check debug_print */
    {
        return 3;                                                                    /* return error */
    }
    if (handle->iic_init == NULL)                                                    /* check iic_init */
    {
        handle->debug_print("stcc4: iic_init is null.\n");                           /* iic_init is null */
    
        return 3;                                                                    /* return error */
    }
    if (handle->iic_deinit == NULL)                                                  /* check iic_deinit */
    {
        handle->debug_print("stcc4: iic_deinit is null.\n");                         /* iic_deinit is null */
    
        return 3;                                                                    /* return error */
    }
    if (handle->iic_write_cmd == NULL)                                               /* check iic_write_cmd */
    {
        handle->debug_print("stcc4: iic_write_cmd is null.\n");                      /* iic_write_cmd is null */
    
        return 3;                                                                    /* return error */
    }
    if (handle->iic_read_cmd == NULL)                                                /* check iic_read_cmd */
    {
        handle->debug_print("stcc4: iic_read_cmd is null.\n");                       /* iic_read_cmd is null */
    
        return 3;                                                                    /* return error */
    }
    if (handle->delay_ms == NULL)                                                    /* check delay_ms */
    {
        handle->debug_print("stcc4: delay_ms is null.\n");                           /* delay_ms is null */
    
        return 3;                                                                    /* return error */
    }
    
    if (handle->iic_init() != 0)                                                     /* iic init */
    {
        handle->debug_print("stcc4: iic init failed.\n");                            /* iic init failed */
    
        return 1;                                                                    /* return error */
    }
    
    reg = STCC4_COMMAND_PERFORM_SOFT_RESET;                                          /* perform soft reset command */
    (void)handle->iic_write_cmd(0x00, &reg, 1);                                      /* perform soft reset */
    handle->delay_ms(10);                                                            /* wait 10ms */
    
    res = a_stcc4_iic_read(handle, STCC4_COMMAND_GET_PRODUCT_ID, buf, 18, 1);        /* write config */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("stcc4: get product id failed.\n");                      /* get product id failed */
        (void)handle->iic_deinit();                                                  /* iic deinit */
        
        return 4;                                                                    /* return error */
    }
    for (i = 0; i < 18; i += 3)                                                      /* check all */
    {
        if (a_stcc4_generate_crc(buf + i, 2) != buf[i + 2])                          /* check crc */
        {
            handle->debug_print("stcc4: crc check failed.\n");                       /* crc check failed */
            (void)handle->iic_deinit();                                              /* iic deinit */
            
            return 4;                                                                /* return error */
        }
    }
    product_id = ((uint32_t)buf[0] << 24) | ((uint32_t)buf[1] << 16) |
                  ((uint32_t)buf[3] << 8) | ((uint32_t)buf[4] << 0);                 /* set product id */
    if (product_id != 0x0901018AU)                                                   /* check product id */
    {
        handle->debug_print("stcc4: product id is invalid.\n");                      /* id is invalid */
        (void)handle->iic_deinit();                                                  /* iic deinit */
        
        return 4;                                                                    /* return error */
    }
    
    handle->inited = 1;                                                              /* flag finish initialization */
  
    return 0;                                                                        /* success return 0 */
}

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
uint8_t stcc4_deinit(stcc4_handle_t *handle)
{
    uint8_t res;
    
    if (handle == NULL)                                                                         /* check handle */
    {
        return 2;                                                                               /* return error */
    }
    if (handle->inited != 1)                                                                    /* check handle initialization */
    {
        return 3;                                                                               /* return error */
    }    
    
    res = a_stcc4_iic_write(handle, STCC4_COMMAND_STOP_CONTINUOUS_MEASUREMENT, NULL, 0);        /* write config */
    if (res != 0)                                                                               /* check result */
    {
        handle->debug_print("stcc4: stop continuous measurement failed.\n");                    /* stop continuous measurement failed */
       
        return 4;                                                                               /* return error */
    }
    if (handle->iic_deinit() != 0)                                                              /* iic deinit */
    {
        handle->debug_print("stcc4: iic close failed.\n");                                      /* iic close failed */
    
        return 3;                                                                               /* return error */
    }
    handle->inited = 0;                                                                         /* flag close initialization */
  
    return 0;                                                                                   /* success return 0 */
}

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
uint8_t stcc4_frc_co2_convert_to_register(stcc4_handle_t *handle, float ppm, uint16_t *reg)
{
    if (handle == NULL)                       /* check handle */
    {
        return 2;                             /* return error */
    }
    if (handle->inited != 1)                  /* check handle initialization */
    {
        return 3;                             /* return error */
    }
    
    *reg = (uint16_t)(ppm + 32768.0f);        /* convert real data to register data */
    
    return 0;                                 /* success return 0 */
}

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
uint8_t stcc4_frc_co2_convert_to_data(stcc4_handle_t *handle, uint16_t reg, float *ppm)
{
    if (handle == NULL)                    /* check handle */
    {
        return 2;                          /* return error */
    }
    if (handle->inited != 1)               /* check handle initialization */
    {
        return 3;                          /* return error */
    }
    
    *ppm = (float)(reg) - 32768.0f;        /* convert raw data to real data */
    
    return 0;                              /* success return 0 */
}

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
uint8_t stcc4_humidity_convert_to_register(stcc4_handle_t *handle, float percentage, uint16_t *reg)
{
    if (handle == NULL)                                              /* check handle */
    {
        return 2;                                                    /* return error */
    }
    if (handle->inited != 1)                                         /* check handle initialization */
    {
        return 3;                                                    /* return error */
    }
    
    *reg = (uint16_t)((percentage + 6.0f) * 65535.0f / 125.0f);      /* convert real data to register data */
    
    return 0;                                                        /* success return 0 */
}

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
uint8_t stcc4_humidity_convert_to_data(stcc4_handle_t *handle, uint16_t reg, float *percentage)
{
    if (handle == NULL)                                           /* check handle */
    {
        return 2;                                                 /* return error */
    }
    if (handle->inited != 1)                                      /* check handle initialization */
    {
        return 3;                                                 /* return error */
    }
    
    *percentage = (float)(reg) / 65535.0f * 125.0f - 6.0f;        /* convert raw data to real data */
    
    return 0;                                                     /* success return 0 */
}

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
uint8_t stcc4_temperature_convert_to_register(stcc4_handle_t *handle, float deg, uint16_t *reg)
{
    if (handle == NULL)                                        /* check handle */
    {
        return 2;                                              /* return error */
    }
    if (handle->inited != 1)                                   /* check handle initialization */
    {
        return 3;                                              /* return error */
    }
    
    *reg = (uint16_t)((deg + 45.0f) * 65535.0f / 175.0f);      /* convert real data to register data */
    
    return 0;                                                  /* success return 0 */
}

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
uint8_t stcc4_temperature_convert_to_data(stcc4_handle_t *handle, uint16_t reg, float *deg)
{
    if (handle == NULL)                                     /* check handle */
    {
        return 2;                                           /* return error */
    }
    if (handle->inited != 1)                                /* check handle initialization */
    {
        return 3;                                           /* return error */
    }
    
    *deg = (float)(reg) / 65535.0f * 175.0f - 45.0f;        /* convert raw data to real data */
    
    return 0;                                               /* success return 0 */
}

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
uint8_t stcc4_pressure_convert_to_register(stcc4_handle_t *handle, float pa, uint16_t *reg)
{
    if (handle == NULL)                  /* check handle */
    {
        return 2;                        /* return error */
    }
    if (handle->inited != 1)             /* check handle initialization */
    {
        return 3;                        /* return error */
    }
    
    *reg = (uint16_t)(pa / 2.0f);        /* convert real data to register data */
    
    return 0;                            /* success return 0 */
}

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
uint8_t stcc4_pressure_convert_to_data(stcc4_handle_t *handle, uint16_t reg, float *pa)
{
    if (handle == NULL)             /* check handle */
    {
        return 2;                   /* return error */
    }
    if (handle->inited != 1)        /* check handle initialization */
    {
        return 3;                   /* return error */
    }
    
    *pa = (float)(reg) * 2;         /* convert raw data to real data */
    
    return 0;                       /* success return 0 */
}

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
uint8_t stcc4_set_reg(stcc4_handle_t *handle, uint16_t reg, uint8_t *buf, uint16_t len)
{
    if (handle == NULL)                                       /* check handle */
    {
        return 2;                                             /* return error */
    }
    if (handle->inited != 1)                                  /* check handle initialization */
    {
        return 3;                                             /* return error */
    }
  
    return a_stcc4_iic_write(handle, reg, buf, len);          /* write data */
}

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
uint8_t stcc4_get_reg(stcc4_handle_t *handle, uint16_t reg, uint8_t *buf, uint16_t len, uint16_t delay_ms)
{
    if (handle == NULL)                                            /* check handle */
    {
        return 2;                                                  /* return error */
    }
    if (handle->inited != 1)                                       /* check handle initialization */
    {
        return 3;                                                  /* return error */
    }
  
    return a_stcc4_iic_read(handle, reg, buf, len, delay_ms);      /* read data */
}

/**
 * @brief      get chip information
 * @param[out] *info pointer to an stcc4 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t stcc4_info(stcc4_info_t *info)
{
    if (info == NULL)                                               /* check handle */
    {
        return 2;                                                   /* return error */
    }
    
    memset(info, 0, sizeof(stcc4_info_t));                          /* initialize stcc4 info structure */
    strncpy(info->chip_name, CHIP_NAME, 32);                        /* copy chip name */
    strncpy(info->manufacturer_name, MANUFACTURER_NAME, 32);        /* copy manufacturer name */
    strncpy(info->interface, "IIC", 8);                             /* copy interface name */
    info->supply_voltage_min_v = SUPPLY_VOLTAGE_MIN;                /* set minimal supply voltage */
    info->supply_voltage_max_v = SUPPLY_VOLTAGE_MAX;                /* set maximum supply voltage */
    info->max_current_ma = MAX_CURRENT;                             /* set maximum current */
    info->temperature_max = TEMPERATURE_MAX;                        /* set minimal temperature */
    info->temperature_min = TEMPERATURE_MIN;                        /* set maximum temperature */
    info->driver_version = DRIVER_VERSION;                          /* set driver version */
    
    return 0;                                                       /* success return 0 */
}
