/*
 * misc_sx126x_config.h
 *
 *  Created on: May 20, 2025
 *      Author: Dyulabs Pvt. Ltd
 *
 * misc_sx126x_config.h - Misc SX126x Config
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

#ifndef MISC_SX126X_CONFIG_H
#define MISC_SX126X_CONFIG_H

#include "sx126x.h"

#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************************************************************
 *                                      Important Macro Defines                                       *
 ******************************************************************************************************/
/* SX126X Default SYNCWORD*/
#define DU_SX126X_DFT_SYNC_WORD 0x12

/* SX126X CONFIG ----------------------------------------------------------------*/
#define DU_SX126X_STANDBY_MODE SX126X_STANDBY_CFG_RC // Standby Mode in RC
#define DU_SX126X_REG_MODE SX126X_REG_MODE_DCDC      // Power Regulator Mode in DCDC

/* Power Amplifier Configuration -------------------------------------------------*/
#define DU_SX126X_PA_CFG_PARAMS_HP_MAX 0x07        // PA output power level, 0x00 to 0x07
#define DU_SX126X_PA_CFG_PARAMS_PA_DUTY_CYCLE 0x07 // PA output power level, 0x04 to 0x07
#define DU_SX126X_PA_CFG_PARAMS_DEVICE_SEL 0x00    // 0x00 for SX1262, 0x01 for SX1261
#define DU_SX126X_PA_CFG_PARAMS_PA_LUT 0x01        // Power Amplifier Lookup Table

/* TCXO Control Configuration -------------------------------------------------*/
#define DU_SX126X_TCXO_CTRL_VOLTAGE SX126X_TCXO_CTRL_3_3V // TCXO Control Voltage
#define DU_SX126X_TCXO_CTRL_DELAY 100                     // TCXO Control Delay

/* RF Switch Control Configuration -------------------------------------------------*/
// Set the RF switch control pins through DIO2
#define DU_SX126X_DIO2_RF_SW_CTRL 1 // Enable (1) or Disable (0)

/* Packet Configuration -------------------------------------------------*/
#define DU_SX126X_PKT_TYPE SX126X_PKT_TYPE_LORA // Packet Type
#define DU_SX126X_RF_FREQ 867000000             // RF Frequency in Hz

/* TX Configuration -------------------------------------------------*/
// Set the TX power to 22 dBm, ramp time 200 us
// The ramp time is the time it takes for the PA to go from idle to TX power
// Faster ramp times can cause spurious emissions, while slower times improve signal stability.
// ramp time -> Balance between power and stability
#define DU_SX126X_TX_POWER 22                 // TX Power
#define DU_SX126X_RAMP_TIME SX126X_RAMP_10_US // Ramp Time

/* LoRa Modulation Configuration -------------------------------------------------*/
#define DU_SX126X_LORA_MOD_PARAMS_BW SX126X_LORA_BW_125 // Bandwidth
#define DU_SX126X_LORA_MOD_PARAMS_SF SX126X_LORA_SF9    // Spreading Factor
#define DU_SX126X_LORA_MOD_PARAMS_CR SX126X_LORA_CR_4_8 // Coding Rate
#define DU_SX126X_LORA_MOD_PARAMS_LDRO 1                // Low Data Rate Optimization

/* LoRa Packet Configuration -------------------------------------------------*/
#define DU_SX126X_LORA_PKT_PARAMS_PREAMBLE_LEN_IN_SYMB 8               // Preamble Length in Symbols
#define DU_SX126X_LORA_PKT_PARAMS_HEADER_TYPE SX126X_LORA_PKT_EXPLICIT // Header Type
#define DU_SX126X_LORA_PKT_PARAMS_PLD_LEN_IN_BYTES 255                 // Payload Length in Bytes
#define DU_SX126X_LORA_PKT_PARAMS_CRC_IS_ON true                       // CRC is ON
#define DU_SX126X_LORA_PKT_PARAMS_INVERT_IQ_IS_ON false                // Invert IQ is OFF

/* CAD Configuration -------------------------------------------------*/
#define DU_SX126X_CAD_PARAMS_CAD_SYMB_NUM SX126X_CAD_16_SYMB // CAD Symbol Number
#define DU_SX126X_CAD_PARAMS_CAD_DETECT_PEAK 25              // CAD Detect Peak
#define DU_SX126X_CAD_PARAMS_CAD_DETECT_MIN 10               // CAD Detect Min
#define DU_SX126X_CAD_PARAMS_CAD_EXIT_MODE SX126X_CAD_ONLY   // CAD Exit Mode
#define DU_SX126X_CAD_PARAMS_CAD_TIMEOUT 300                 // CAD Timeout

/* RX Configuration -------------------------------------------------*/
#define DU_SX126X_RX_BOOSTED true // RX Boosted

#define DU_SX126X_CHANNEL_CLEAR_ATTEMPTS 10 // Channel Clear Attempts

#ifdef __cplusplus
}
#endif

#endif /* MISC_SX126X_CONFIG_H */
