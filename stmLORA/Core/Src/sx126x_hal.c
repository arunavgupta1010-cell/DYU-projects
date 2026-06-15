/*
 * sx126x_hal.c
 *
 *  Created on: Feb 10, 2025
 *      Author: Dyulabs Pvt. Ltd
 *
 * sx126x_hal.c - SX126x HAL
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

/********************************************************************************************************
 *                                            Includes                                                  *
 ********************************************************************************************************/

/**********************************************************
 *                  local libraries                      *
 **********************************************************/

/* Middleware Libraries */
#include "sx126x_hal.h"
#include "misc_sx126x_hal.h"

/********************************************************************************************************
 *                                      Static defines, Global variables                                *
 ********************************************************************************************************/

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/********************************************************************************************************
 *                                          Functions Declaration                                       *
 ********************************************************************************************************/

/********************************************************************************************************
 *                                                Functions                                             *
 ********************************************************************************************************/

//*****************************************************************************
//
//! \brief Write data to the SX126x device
//!
//! \param context is the context of the SX126x device
//! \param command is the command to be written
//! \param command_length is the length of the command
//! \param data is the data to be written
//! \param data_length is the length of the data
//!
//! \return Operation status
//
//*****************************************************************************/
sx126x_hal_status_t sx126x_hal_write(const void *context, const uint8_t *command, const uint16_t command_length,
                                     const uint8_t *data, const uint16_t data_length)
{
    bool status;
    status = misc_sx162x_hal_write(command, command_length, data, data_length);
    if (status == false)
    {
        return SX126X_HAL_STATUS_ERROR;
    }
    return SX126X_HAL_STATUS_OK;
}

//*****************************************************************************
//
//! \brief Read data from the SX126x device
//!
//! \param context is the context of the SX126x device
//! \param command is the command to be read
//! \param command_length is the length of the command
//! \param data is the data to be read
//! \param data_length is the length of the data
//!
//! \return Operation status
//
//*****************************************************************************/
sx126x_hal_status_t sx126x_hal_read(const void *context, const uint8_t *command, const uint16_t command_length,
                                    uint8_t *data, const uint16_t data_length)
{
    bool status;
    status = misc_sx162x_hal_read(command, command_length, data, data_length);
    if (status == false)
        return SX126X_HAL_STATUS_ERROR;
    return SX126X_HAL_STATUS_OK;
}

//*****************************************************************************
//
//! \brief Reset the SX126x device
//!
//! \param context is the context of the SX126x device
//!
//! \return Operation status
//
//*****************************************************************************/
sx126x_hal_status_t sx126x_hal_reset(const void *context)
{
    bool status;
    status = misc_sx126x_hal_reset();
    if (status == false)
        return SX126X_HAL_STATUS_ERROR;
    return SX126X_HAL_STATUS_OK;
}

//*****************************************************************************
//
//! \brief Wake the SX126x device up
//!
//! \param context is the context of the SX126x device
//!
//! \return Operation status
//
//*****************************************************************************/
sx126x_hal_status_t sx126x_hal_wakeup(const void *context)
{
    bool status;
    status = misc_sx126x_hal_wakeup();
    if (status == false)
        return SX126X_HAL_STATUS_ERROR;
    return SX126X_HAL_STATUS_OK;
}
