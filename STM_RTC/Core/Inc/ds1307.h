#ifndef DS1307_H
#define DS1307_H

#include "stm32g0xx_hal.h"   // apne MCU ke HAL header ke hisaab se change karo

#define DS1307_ADDR (0x68 << 1)   // Correct address
typedef struct {
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    uint8_t date;
    uint8_t month;
    uint8_t year;
} DS1307_Time;

uint8_t bcd2dec(uint8_t val);
uint8_t dec2bcd(uint8_t val);

HAL_StatusTypeDef DS1307_SetTime(I2C_HandleTypeDef *hi2c, DS1307_Time *t);
HAL_StatusTypeDef DS1307_GetTime(I2C_HandleTypeDef *hi2c, DS1307_Time *t);

#endif
