#include "ds1307.h"

uint8_t bcd2dec(uint8_t val) { return ((val >> 4) * 10 + (val & 0x0F)); }
uint8_t dec2bcd(uint8_t val) { return ((val / 10) << 4) + (val % 10); }

HAL_StatusTypeDef DS1307_SetTime(I2C_HandleTypeDef *hi2c, DS1307_Time *t) {
    uint8_t data[8];
    data[0] = 0x00; // start register address

    // CH bit clear
    data[1] = dec2bcd(t->seconds) & 0x7F;  // 0x7F mask, not 0x8F
    data[2] = dec2bcd(t->minutes);
    data[3] = dec2bcd(t->hours) & 0x3F;    // force 24h mode
    data[4] = 0x01; // day of week dummy
    data[5] = dec2bcd(t->date);
    data[6] = dec2bcd(t->month);
    data[7] = dec2bcd(t->year);

    return HAL_I2C_Master_Transmit(hi2c, DS1307_ADDR, data, 8, HAL_MAX_DELAY);
}

HAL_StatusTypeDef DS1307_GetTime(I2C_HandleTypeDef *hi2c, DS1307_Time *t) {
    uint8_t reg = 0x00;
    uint8_t data[7];

    if (HAL_I2C_Master_Transmit(hi2c, DS1307_ADDR, &reg, 1, HAL_MAX_DELAY) != HAL_OK)
        return HAL_ERROR;

    if (HAL_I2C_Master_Receive(hi2c, DS1307_ADDR, data, 7, HAL_MAX_DELAY) != HAL_OK)
        return HAL_ERROR;

    t->seconds = bcd2dec(data[0] & 0x7F);
    t->minutes = bcd2dec(data[1]);
    t->hours   = bcd2dec(data[2] & 0x3F);
    t->date    = bcd2dec(data[4]);
    t->month   = bcd2dec(data[5]);
    t->year    = bcd2dec(data[6]);

    return HAL_OK;
}
