/*
 * misc_sx126x_hal.c
 *
 *  Created on: May 20, 2025
 *      Author: Dyulabs Pvt. Ltd
 *
 * misc_sx126x_hal.c - Misc SX126x HAL Source
 * Copyright (c) 2025-26 Dyulabs Pvt. Ltd.  All right reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <main.h>
extern SPI_HandleTypeDef hspi1;



#define SPI_TIMEOUT 2000   // Milli Seconds
#define MODEM_TIMEOUT 5000 // Millis Seconds

static bool misc_sx126x_hal_select(void);
static bool misc_sx126x_hal_deselect(void);

bool misc_sx126x_hal_ready(uint32_t timeout)
{
    uint32_t start_time = HAL_GetTick ();

    // Continuously check if the BUSY pin goes LOW within the timeout period
    while (HAL_GetTick () - start_time < timeout)
    {
        	if (HAL_GPIO_ReadPin(LORA_BUSY_GPIO_Port,LORA_BUSY_Pin) == GPIO_PIN_RESET )// Check if BUSY pin is LOW
        {
            return true; // SX126x is ready
        }
    }
    return false; // Timeout occurred, SX126x is still busy
}

bool misc_sx126x_hal_init(uint16_t nss, uint16_t busy, uint16_t dio1, uint16_t reset)
{
    HAL_GPIO_WritePin(LORA_NSS_GPIO_Port, LORA_NSS_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LORA_RESET_GPIO_Port, LORA_RESET_Pin, GPIO_PIN_SET);
    return true;
}

static bool misc_sx126x_hal_select(void)
{
	 HAL_GPIO_WritePin(LORA_NSS_GPIO_Port, LORA_NSS_Pin,GPIO_PIN_RESET);


    return true;
}

static bool misc_sx126x_hal_deselect(void)
{
	 HAL_GPIO_WritePin(LORA_NSS_GPIO_Port, LORA_NSS_Pin,GPIO_PIN_SET);

    return true;
}

bool misc_sx162x_hal_write(const uint8_t *command, const uint16_t command_length,
                           const uint8_t *data, const uint16_t data_length)
{
    bool status;

    status = misc_sx126x_hal_ready(MODEM_TIMEOUT);
    if (status == false)
    {
        printf("WARNING: SX126X is BUSY\r\n");
        return false;
    }

    if (misc_sx126x_hal_select() == false)
    {
        return false;
    }

    if (command_length > 0)
    {
        if (HAL_SPI_Transmit(&hspi1, (uint8_t *)command, command_length, SPI_TIMEOUT) != HAL_OK)
        {
            printf("misc command transmit write failed\r\n");
            misc_sx126x_hal_deselect();
            return false;
        }
    }
    if (data_length > 0)
    {
        if (HAL_SPI_Transmit(&hspi1, (uint8_t *)data, data_length, SPI_TIMEOUT) != HAL_OK)
        {
            printf("misc data transmit write failed\r\n");
            misc_sx126x_hal_deselect();
            return false;
        }
    }

    status = misc_sx126x_hal_deselect();
    return status;
}

bool misc_sx162x_hal_read(const uint8_t *command, const uint16_t command_length,
                          uint8_t *data, const uint16_t data_length)
{
    bool status = misc_sx126x_hal_ready(MODEM_TIMEOUT);

    if (status == false)
    {
        printf("WARNING: SX126X is BUSY\r\n");
        return false;
    }

    if (misc_sx126x_hal_select() == false)
    {
        return false;
    }

    if (command_length > 0)
    {
        if (HAL_SPI_Transmit(&hspi1, (uint8_t *)command, command_length, SPI_TIMEOUT) != HAL_OK)
        {
            printf("misc transmit ready failed\r\n");
            misc_sx126x_hal_deselect();
            return false;
        }
    }

    if (data_length > 0)
    {
        if (HAL_SPI_Receive(&hspi1, (uint8_t *)data, data_length, SPI_TIMEOUT) != HAL_OK)
        {
            printf("misc received ready failed\r\n");
            misc_sx126x_hal_deselect();
            return false;
        }

    status = misc_sx126x_hal_deselect();
    return status;
}

bool misc_sx126x_hal_reset(void)
{
    HAL_GPIO_WritePin(LORA_RESET_GPIO_Port, LORA_RESET_Pin,GPIO_PIN_RESET);


    HAL_Delay(1);

    HAL_GPIO_WritePin(LORA_RESET_GPIO_Port, LORA_RESET_Pin,GPIO_PIN_SET);

    HAL_Delay(6);
    return true;
}

bool misc_sx126x_hal_wakeup(void)
{
    if (misc_sx126x_hal_select() == false)
    {
        return false;
    }

    HAL_Delay(2);

    if (misc_sx126x_hal_deselect() == false)
    {
        return false;
    }

    return misc_sx126x_hal_ready(MODEM_TIMEOUT);
}

bool misc_sx126x_hal_deinit()
{
	HAL_SPI_DeInit(&hspi1);
    return true;
}
