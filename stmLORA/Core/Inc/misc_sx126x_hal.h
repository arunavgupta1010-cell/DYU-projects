/*
 * misc_sx126x_hal.h
 *
 *  Created on: May 20, 2025
 *      Author: Dyulabs Pvt. Ltd
 *
 * misc_sx126x_hal.h - Misc SX126x HAL Header
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

#ifndef MISC_SX126X_HAL_H
#define MISC_SX126X_HAL_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /********************************************************************************************************
     *                                      Functions Declaration                                           *
     ********************************************************************************************************/
    bool misc_sx126x_hal_reset(void);
    bool misc_sx126x_hal_wakeup(void);
    bool misc_sx126x_hal_init(uint16_t nss, uint16_t busy, uint16_t dio1, uint16_t reset);
    bool misc_sx126x_hal_deinit();
    bool misc_sx162x_hal_read(const uint8_t *command, const uint16_t command_length,
                              uint8_t *data, const uint16_t data_length);

    bool misc_sx162x_hal_write(const uint8_t *command, const uint16_t command_length,
                               const uint8_t *data, const uint16_t data_length);

#ifdef __cplusplus
}
#endif

#endif /* MISC_SX126X_HAL_H */
