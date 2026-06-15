/*
 * misc_sx126x.h
 *
 *  Created on: May 20, 2025
 *      Author: Dyulabs Pvt. Ltd
 *
 * misc_sx126x.h - Misc SX126x Header
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

#ifndef MISC_SX126X_H
#define MISC_SX126X_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

    bool misc_sx126x_init(void (*RxCb)(uint8_t *data, uint16_t len, int16_t rssi, int8_t snr));
    void misc_sx126X_sleep(void);
    void misc_sx126x_wakeup(void);
    void misc_sx126x_deinit(void);
    void misc_sx126x_irq_cb(void);
    bool misc_sx126x_is_rx_mode(void);
    bool misc_sx126x_is_channel_clear(void);
    void misc_sx126x_receive(uint32_t timeout);
    bool misc_sx126x_set_syncword(uint8_t sync_word);
    uint8_t misc_sx126x_get_syncword();
    bool misc_sx126x_transmit(uint8_t *data, uint16_t length, uint32_t timeout);

#ifdef __cplusplus
}
#endif

#endif /* MISC_SX126X_H */
