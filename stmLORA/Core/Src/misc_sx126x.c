/*
 * misc_sx126x.c
 *
 *  Created on: May 20, 2025
 *      Author: Dyulabs Pvt. Ltd
 *
 * misc_sx126x.c - Misc SX126x Source
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
#include <stddef.h>
#include <stdio.h>
#include "main.h"
#include "misc_sx126x.h"
#include "misc_sx126x_hal.h"
#include "misc_sx126x_config.h"

#define DU_RADIO_MAX_BUFFER_SIZE 255

static bool gf_tx_done = false;
static bool gf_rx_mode = false;
static bool gf_cad_done = false;
static bool gf_cad_detect = false;
static uint32_t gt_sync_timer = 0;

static uint8_t g_buffer_rx[DU_RADIO_MAX_BUFFER_SIZE] = {0};

static void (*RxCpltCallbackRadio)(uint8_t *data, uint16_t len, int16_t rssi, int8_t snr);

static uint8_t compute_lora_ldro(sx126x_lora_sf_t sf, sx126x_lora_bw_t bw)
{
    switch (bw)
    {
    case SX126X_LORA_BW_500:
        return 0;

    case SX126X_LORA_BW_250:
        return (sf == SX126X_LORA_SF12) ? 1 : 0;

    case SX126X_LORA_BW_125:
        return ((sf == SX126X_LORA_SF12) || (sf == SX126X_LORA_SF11)) ? 1 : 0;

    case SX126X_LORA_BW_062:
        return ((sf == SX126X_LORA_SF12) || (sf == SX126X_LORA_SF11) ||
                (sf == SX126X_LORA_SF10))
                   ? 1
                   : 0;

    case SX126X_LORA_BW_041:
        return ((sf == SX126X_LORA_SF12) || (sf == SX126X_LORA_SF11) ||
                (sf == SX126X_LORA_SF10) || (sf == SX126X_LORA_SF9))
                   ? 1
                   : 0;

    case SX126X_LORA_BW_031:
    case SX126X_LORA_BW_020:
    case SX126X_LORA_BW_015:
    case SX126X_LORA_BW_010:
    case SX126X_LORA_BW_007:
        return 1;

    default:
        return 0;
    }
}

bool misc_sx126x_init(void (*RxCb)(uint8_t *data, uint16_t len, int16_t rssi, int8_t snr))
{
    bool status;
    sx126x_status_t modem_status;
    RxCpltCallbackRadio = RxCb;

    status = misc_sx126x_hal_init(0, 0, 0, 0);
    if (status == false)
    {
        printf( "ERROR: SX126X-HAL-INIT\r\n");
        return false;
    }

    status = misc_sx126x_hal_reset();
    if (status == false)
    {
        printf( "ERROR: SX126X-HAL-RST\r\n");
        return false;
    }

    status = misc_sx126x_hal_wakeup();
    if (status == false)
    {
        printf( "ERROR: SX126X-HAL-WAKEUP\r\n");
        return false;
    }

    modem_status = sx126x_set_standby(NULL, DU_SX126X_STANDBY_MODE);
    if (modem_status == SX126X_STATUS_ERROR)
    {
        printf( "ERROR: SX126X-SET-STANDBY\r\n");
        return false;
    }

    modem_status = sx126x_set_reg_mode(NULL, DU_SX126X_REG_MODE);
    if (modem_status == SX126X_STATUS_ERROR)
    {
        printf( "ERROR: SX126X-HAL-SET-REG-MODE\r\n");
        return false;
    }

    sx126x_pa_cfg_params_t pa_cfg_params = {
        .hp_max = DU_SX126X_PA_CFG_PARAMS_HP_MAX,
        .pa_duty_cycle = DU_SX126X_PA_CFG_PARAMS_PA_DUTY_CYCLE,
        .device_sel = DU_SX126X_PA_CFG_PARAMS_DEVICE_SEL,
        .pa_lut = DU_SX126X_PA_CFG_PARAMS_PA_LUT,
    };
    modem_status = sx126x_set_pa_cfg(NULL, &pa_cfg_params);
    if (modem_status == SX126X_STATUS_ERROR)
    {
        printf( "ERROR: SX126X-SET-PA\r\n");
        return false;
    }

#if 0
    modem_status = sx126x_set_dio3_as_tcxo_ctrl(NULL, DU_SX126X_TCXO_CTRL_VOLTAGE, DU_SX126X_TCXO_CTRL_DELAY);
    if (modem_status == SX126X_STATUS_ERROR)
    {
        printf( "ERROR: SX126X-SET-DIO3-AS-TCXO\r\n");
        return false;
    }
#endif
    modem_status = sx126x_set_dio2_as_rf_sw_ctrl(NULL, DU_SX126X_DIO2_RF_SW_CTRL);
    if (modem_status == SX126X_STATUS_ERROR)
    {
        printf( "ERROR: SX126X-SET-DIO2-AS-RF-SWITCH\r\n");
        return false;
    }
    modem_status = sx126x_set_pkt_type(NULL, DU_SX126X_PKT_TYPE);
    if (modem_status == SX126X_STATUS_ERROR)
    {
        printf( "ERROR: SX126X-SET-PKT-TYPE\r\n");
        return false;
    }
    modem_status =
        sx126x_set_rf_freq(NULL, DU_SX126X_RF_FREQ);
    if (modem_status == SX126X_STATUS_ERROR)
    {
        printf( "ERROR: SX126X-SET-RF-FREQ\r\n");
        return false;
    }

    modem_status = sx126x_set_tx_params(NULL, DU_SX126X_TX_POWER, DU_SX126X_RAMP_TIME);
    if (modem_status == SX126X_STATUS_ERROR)
    {
        printf( "ERROR: SX126X-SET-TX-PARAMS\r\n");
        return false;
    }

    uint8_t ldro_value = compute_lora_ldro(DU_SX126X_LORA_MOD_PARAMS_SF, DU_SX126X_LORA_MOD_PARAMS_BW);

    sx126x_mod_params_lora_t sx126x_mod_params_lora = {
        .bw = DU_SX126X_LORA_MOD_PARAMS_BW,
        .sf = DU_SX126X_LORA_MOD_PARAMS_SF,
        .cr = DU_SX126X_LORA_MOD_PARAMS_CR,
        .ldro = ldro_value};
    modem_status = sx126x_set_lora_mod_params(NULL, &sx126x_mod_params_lora);
    if (modem_status == SX126X_STATUS_ERROR)
    {
        printf( "ERROR: SX126X-SET-MODULATION-PARAMS\r\n");
        return false;
    }

    sx126x_pkt_params_lora_t sx126x_pkt_params_lora = {
            .preamble_len_in_symb = DU_SX126X_LORA_PKT_PARAMS_PREAMBLE_LEN_IN_SYMB,
            .header_type = DU_SX126X_LORA_PKT_PARAMS_HEADER_TYPE,
            .pld_len_in_bytes = DU_SX126X_LORA_PKT_PARAMS_PLD_LEN_IN_BYTES,
            .crc_is_on = DU_SX126X_LORA_PKT_PARAMS_CRC_IS_ON,
            .invert_iq_is_on = DU_SX126X_LORA_PKT_PARAMS_INVERT_IQ_IS_ON};
        modem_status = sx126x_set_lora_pkt_params(NULL, &sx126x_pkt_params_lora);
    if (modem_status == SX126X_STATUS_ERROR)
    {
        printf( "ERROR: SX126X-SET-PKT-PARAMS\r\n");
        return false;
    }

    sx126x_cad_params_t cad_params = {
        .cad_symb_nb = DU_SX126X_CAD_PARAMS_CAD_SYMB_NUM,
        .cad_detect_peak = DU_SX126X_CAD_PARAMS_CAD_DETECT_PEAK,
        .cad_detect_min = DU_SX126X_CAD_PARAMS_CAD_DETECT_MIN,
        .cad_exit_mode = DU_SX126X_CAD_PARAMS_CAD_EXIT_MODE,
        .cad_timeout = DU_SX126X_CAD_PARAMS_CAD_TIMEOUT,
    };
    modem_status = sx126x_set_cad_params(NULL, &cad_params);
    if (modem_status == SX126X_STATUS_ERROR)
    {
        printf( "ERROR: SX126X-SET-CAD-PARAMS\r\n");
        return false;
    }

    modem_status = sx126x_cfg_rx_boosted(NULL, DU_SX126X_RX_BOOSTED);
    if (modem_status == SX126X_STATUS_ERROR)
    {
        printf( "ERROR: SX126X-CFG-RX-BOOST\r\n");
        return false;
    }

    // test whether syncword is being set or not:
    status = misc_sx126x_set_syncword(DU_SX126X_DFT_SYNC_WORD);
    if (status == false)
    {
        printf( "FAIL: SET-DFT-SYNC-WORD\r\n");
    }

    return true;
}

void misc_sx126x_deinit(void)
{
    misc_sx126x_hal_deinit();
}

bool misc_sx126x_set_syncword(uint8_t sync_word)
{
    bool status;
    sx126x_set_standby(NULL, DU_SX126X_STANDBY_MODE);

    // consider 0x12 for any sync word below 0x12
    if ((sync_word < DU_SX126X_DFT_SYNC_WORD) || (sync_word == 0xFF))
    {
        sx126x_set_lora_sync_word(NULL, DU_SX126X_DFT_SYNC_WORD);
        if (misc_sx126x_get_syncword() == DU_SX126X_DFT_SYNC_WORD)
        {
            status = true;
        }
        else
        {
            status = false;
        }
    }

    // if requested sync word is multiple of 16 than use syncword 1-less than of it.
    else if (sync_word % 16 == 0)
    {
        sx126x_set_lora_sync_word(NULL, sync_word - 1);
        if (misc_sx126x_get_syncword() == (sync_word - 1))
        {
            status = true;
        }
        else
        {
            status = false;
        }
    }
    // if requested sync word is multiple of 17 than use syncword 1-more than of it.
    else if (sync_word % 17 == 0)
    {
        sx126x_set_lora_sync_word(NULL, sync_word + 1);
        if (misc_sx126x_get_syncword() == (sync_word + 1))
        {
            status = true;
        }
        else
        {
            status = false;
        }
    }
    else
    {
        sx126x_set_lora_sync_word(NULL, sync_word);
        if (misc_sx126x_get_syncword() == sync_word)
        {
            status = true;
        }
        else
        {
            status = false;
        }
    }
    gf_rx_mode = false;
    return status;
}

uint8_t misc_sx126x_get_syncword()
{
    return sx126x_get_lora_sync_word(NULL);
}

bool misc_sx126x_transmit(uint8_t *data, uint16_t length, uint32_t timeout)
{
    gf_rx_mode = false;
    gf_tx_done = false;
    misc_sx126x_is_channel_clear();
#if defined(DU_LORA_RXEN_PIN)
    du_hal_err_t err;
    err = du_hal_gpio_set(DU_LORA_RXEN_PIN, DU_HAL_GPIO_STATE_HIGH);
    if (err != DU_HAL_OK)
    {
        printf( "ERROR: HAL-GPIO-LORA-RXEN-PIN\r\n");
    }
#endif
#if defined(DU_LORA_TXEN_PIN)
    err = du_hal_gpio_set(DU_LORA_TXEN_PIN, DU_HAL_GPIO_STATE_LOW);
    if (err != DU_HAL_OK)
    {
        printf( "ERROR: HAL-GPIO-LORA-TXEN-PIN\r\n");
    }
#endif
    sx126x_pkt_params_lora_t sx126x_pkt_params_lora = {
        .preamble_len_in_symb = DU_SX126X_LORA_PKT_PARAMS_PREAMBLE_LEN_IN_SYMB,
        .header_type = DU_SX126X_LORA_PKT_PARAMS_HEADER_TYPE,
        .pld_len_in_bytes = length,
        .crc_is_on = DU_SX126X_LORA_PKT_PARAMS_CRC_IS_ON,
        .invert_iq_is_on = DU_SX126X_LORA_PKT_PARAMS_INVERT_IQ_IS_ON};
    sx126x_set_lora_pkt_params(NULL, &sx126x_pkt_params_lora);

    sx126x_set_standby(NULL, DU_SX126X_STANDBY_MODE);
    sx126x_set_buffer_base_address(NULL, 0, 0);
    sx126x_write_buffer(NULL, 0, data, length);
    sx126x_clear_irq_status(NULL, SX126X_IRQ_ALL);
    sx126x_set_dio_irq_params(NULL,
                              SX126X_IRQ_TX_DONE | SX126X_IRQ_TIMEOUT,
                              SX126X_IRQ_TX_DONE | SX126X_IRQ_TIMEOUT,
                              0, 0);
    sx126x_set_tx(NULL, timeout);
    uint16_t temp = timeout;
    while (!gf_tx_done && temp)
    {
        temp--;
        // Wait for the TX to be done
        HAL_Delay(1);
    }

    return gf_tx_done;
}

void misc_sx126x_receive(uint32_t timeout)
{
    if (gf_rx_mode && (uint32_t)(HAL_GetTick() - gt_sync_timer) > 1000)
    {
        gt_sync_timer = HAL_GetTick();

        // Validate: is modem state and gf_rx_mode matches:
        if (!misc_sx126x_is_rx_mode())
        {
            printf( "WARNING: SX126X not is RX-Mode\r\n");
            misc_sx126x_irq_cb();
        }
    }

    if (gf_rx_mode)
    {
        return;
    }

    sx126x_set_standby(NULL, DU_SX126X_STANDBY_MODE);
    sx126x_set_buffer_base_address(NULL, 0, 0);
    sx126x_clear_irq_status(NULL, SX126X_IRQ_ALL);
    sx126x_set_dio_irq_params(NULL,
                              SX126X_IRQ_RX_DONE | SX126X_IRQ_CRC_ERROR | SX126X_IRQ_TIMEOUT | SX126X_IRQ_HEADER_VALID | SX126X_IRQ_HEADER_ERROR | SX126X_IRQ_PREAMBLE_DETECTED | SX126X_IRQ_SYNC_WORD_VALID,
                              SX126X_IRQ_RX_DONE | SX126X_IRQ_CRC_ERROR | SX126X_IRQ_TIMEOUT | SX126X_IRQ_HEADER_VALID | SX126X_IRQ_HEADER_ERROR | SX126X_IRQ_PREAMBLE_DETECTED | SX126X_IRQ_SYNC_WORD_VALID,
                              0, 0);
    sx126x_set_rx(NULL, timeout);
    gf_rx_mode = true;
}

bool misc_sx126x_is_rx_mode(void)
{
    sx126x_chip_status_t radio_status;
    sx126x_get_status(NULL, &radio_status);
    if (radio_status.chip_mode == SX126X_CHIP_MODE_RX)
    {
        gf_rx_mode = true;
        return true;
    }

    gf_rx_mode = false;
    return false;
}

bool misc_sx126x_is_channel_clear(void)
{
    gf_rx_mode = false;
    gf_cad_detect = false;

    for (uint8_t cnt = 0; cnt < DU_SX126X_CHANNEL_CLEAR_ATTEMPTS; cnt++)
    {
        gf_cad_done = false;
        sx126x_set_standby(NULL, DU_SX126X_STANDBY_MODE);
        sx126x_set_dio_irq_params(NULL,
                                  SX126X_IRQ_CAD_DETECTED | SX126X_IRQ_CAD_DONE,
                                  SX126X_IRQ_CAD_DETECTED | SX126X_IRQ_CAD_DONE,
                                  0, 0);
        sx126x_clear_irq_status(NULL, SX126X_IRQ_ALL);
        sx126x_set_cad(NULL);
        uint16_t timeout = 300;
        while (!gf_cad_done && timeout)
        {
            timeout--;
            HAL_Delay(1);
        }

        if (gf_cad_detect)
        {
           printf( "RF-CH-X:%d\r\n", cnt);
            gf_cad_detect = false;
        }
        else
        {
            printf( "RF-CH-CLR\r\n");
            return true;
        }
    }
    // Channel not clear
    printf( "RF-CH-x\r\n");
    return false;
}

void misc_sx126X_sleep(void)
{
    sx126x_set_sleep(NULL, SX126X_SLEEP_CFG_COLD_START);
}

void misc_sx126x_wakeup(void)
{
    sx126x_wakeup(NULL);
}

void misc_sx126x_irq_cb(void)
{
    sx126x_irq_mask_t irq_mask;

    sx126x_get_irq_status(NULL, &irq_mask);
    sx126x_clear_irq_status(NULL, SX126X_IRQ_ALL);

    if ((irq_mask & SX126X_IRQ_TX_DONE) == SX126X_IRQ_TX_DONE)
    {
        printf( "I-RF-TX-DONE\r\n");

        gf_tx_done = true;
        gf_rx_mode = false;
    }
    else if ((irq_mask & SX126X_IRQ_CAD_DETECTED) == SX126X_IRQ_CAD_DETECTED)
    {
        printf( "I-RF-CAD-DET\r\n");

        gf_cad_done = true;
        gf_cad_detect = true;
        gf_rx_mode = false;
    }
    else if ((irq_mask & SX126X_IRQ_CAD_DONE) == SX126X_IRQ_CAD_DONE)
    {
        printf( "I-RF-CAD-DONE\r\n");
        sx126x_set_standby(NULL, DU_SX126X_STANDBY_MODE);

        gf_cad_done = true;
        gf_rx_mode = false;
    }
    else if ((irq_mask & SX126X_IRQ_RX_DONE) == SX126X_IRQ_RX_DONE)
    {
        if ((irq_mask & SX126X_IRQ_PREAMBLE_DETECTED) == SX126X_IRQ_PREAMBLE_DETECTED)
        {
            printf( "I-RF-PREAMBLE-DETECTED\r\n");
        }
        if ((irq_mask & SX126X_IRQ_SYNC_WORD_VALID) == SX126X_IRQ_SYNC_WORD_VALID)
        {
            printf( "I-RF-SYNC-WORD-VALID\r\n");
        }
        if ((irq_mask & SX126X_IRQ_HEADER_VALID) == SX126X_IRQ_HEADER_VALID)
        {
            printf( "I-RF-HEADER-VALID\r\n");
        }
        printf( "I-RF-RX\r\n");

        if ((irq_mask & SX126X_IRQ_CRC_ERROR) == SX126X_IRQ_CRC_ERROR)
        {
            printf( "I-RF-E-CRC\r\n");
        }
        else
        {
            sx126x_set_standby(NULL, DU_SX126X_STANDBY_MODE);

            sx126x_rx_buffer_status_t rx_buffer_status;
            sx126x_get_rx_buffer_status(NULL, &rx_buffer_status);
            sx126x_read_buffer(NULL, rx_buffer_status.buffer_start_pointer, g_buffer_rx, rx_buffer_status.pld_len_in_bytes);

            sx126x_pkt_status_lora_t pkt_status;
            sx126x_get_lora_pkt_status(NULL, &pkt_status);
            sx126x_set_buffer_base_address(NULL, 0, 0);

            RxCpltCallbackRadio(g_buffer_rx, rx_buffer_status.pld_len_in_bytes, pkt_status.rssi_pkt_in_dbm, pkt_status.snr_pkt_in_db);
        }
        gf_rx_mode = false;
    }
    else if ((irq_mask & SX126X_IRQ_TIMEOUT) == SX126X_IRQ_TIMEOUT)
    {
        printf( "I-RF-E-TIMEOUT\r\n");
        gf_rx_mode = false;
    }
    else if ((irq_mask & SX126X_IRQ_PREAMBLE_DETECTED) == SX126X_IRQ_PREAMBLE_DETECTED)
    {
        printf( "I-RF-PREAMBLE-DETECTED\r\n");
        // gf_rx_mode = false;
    }
    else if ((irq_mask & SX126X_IRQ_SYNC_WORD_VALID) == SX126X_IRQ_SYNC_WORD_VALID)
    {
        printf( "I-RF-SYNC-WORD-VALID\r\n");
        // gf_rx_mode = false;
    }
    else if ((irq_mask & SX126X_IRQ_HEADER_VALID) == SX126X_IRQ_HEADER_VALID)
    {
        printf( "I-RF-HEADER-VALID\r\n");
        // gf_rx_mode = false;
    }
    else if ((irq_mask & SX126X_IRQ_HEADER_ERROR) == SX126X_IRQ_HEADER_ERROR)
    {
        printf( "I-RF-E-HEADER\r\n");
        gf_rx_mode = false;
    }
    else
    {
        printf( "E-I-RF-Irq:%d\r\n", irq_mask); // unknown Irq  shouldn't print
        gf_rx_mode = false;
    }
}
