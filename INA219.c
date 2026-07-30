/*
 * INA219.c
 *
 *  Created on: Jul 30, 2026
 *      Author: geeeethb
 */
#include "INA219.h"

#define INA219_I2C_TIMEOUT_MS 100U

static uint16_t INA219_GetCalibrationValue(void) {
    float cal = 0.04096f / (INA219_CURRENT_LSB_A * INA219_SHUNT_OHMS);
    return (uint16_t)cal;
}

HAL_StatusTypeDef INA219_ReadRegister(I2C_HandleTypeDef *hi2c, uint8_t addr7, uint8_t reg, uint16_t *data) {
    uint8_t rx[2];

    HAL_StatusTypeDef ret = HAL_I2C_Mem_Read(hi2c, addr7 << 1, reg, I2C_MEMADD_SIZE_8BIT, rx, 2, INA219_I2C_TIMEOUT_MS);
    if (ret != HAL_OK) return ret;

    *data = ((uint16_t)rx[0] << 8) | rx[1];
    return HAL_OK;
}

HAL_StatusTypeDef INA219_WriteRegister(I2C_HandleTypeDef *hi2c, uint8_t addr7, uint8_t reg, uint16_t data) {
    uint8_t tx[2];

    tx[0] = (uint8_t)(data >> 8);
    tx[1] = (uint8_t)(data & 0xFF);

    return HAL_I2C_Mem_Write(hi2c, addr7 << 1, reg, I2C_MEMADD_SIZE_8BIT, tx, 2, INA219_I2C_TIMEOUT_MS);
}

HAL_StatusTypeDef INA219_Init(I2C_HandleTypeDef *hi2c, uint8_t addr7) {
    HAL_StatusTypeDef ret;

    ret = INA219_WriteRegister(hi2c, addr7, INA219_REG_CONFIG, 0x399F);
    if (ret != HAL_OK) return ret;

    ret = INA219_WriteRegister(hi2c, addr7, INA219_REG_CALIB, INA219_GetCalibrationValue());
    return ret;
}

HAL_StatusTypeDef INA219_ReadBusVoltage_V(I2C_HandleTypeDef *hi2c, uint8_t addr7, float *voltage) {
    uint16_t raw;
    HAL_StatusTypeDef ret = INA219_ReadRegister(hi2c, addr7, INA219_REG_BUS_V, &raw);
    if (ret != HAL_OK) return ret;

    raw >>= 3;
    *voltage = (float)raw * 0.004f;
    return HAL_OK;
}

HAL_StatusTypeDef INA219_ReadShuntVoltage_mV(I2C_HandleTypeDef *hi2c, uint8_t addr7, float *voltage) {
    uint16_t raw;
    HAL_StatusTypeDef ret = INA219_ReadRegister(hi2c, addr7, INA219_REG_SHUNT_V, &raw);
    if (ret != HAL_OK) return ret;

    *voltage = (float)((int16_t)raw) * 0.01f;
    return HAL_OK;
}

HAL_StatusTypeDef INA219_ReadCurrent_A(I2C_HandleTypeDef *hi2c, uint8_t addr7, float *current) {
    uint16_t raw;
    HAL_StatusTypeDef ret;

    ret = INA219_WriteRegister(hi2c, addr7, INA219_REG_CALIB, INA219_GetCalibrationValue());
    if (ret != HAL_OK) return ret;

    ret = INA219_ReadRegister(hi2c, addr7, INA219_REG_CURRENT, &raw);
    if (ret != HAL_OK) return ret;

    *current = (float)((int16_t)raw) * INA219_CURRENT_LSB_A;
    return HAL_OK;
}

HAL_StatusTypeDef INA219_ReadPower_W(I2C_HandleTypeDef *hi2c, uint8_t addr7, float *power) {
    uint16_t raw;
    HAL_StatusTypeDef ret;

    ret = INA219_WriteRegister(hi2c, addr7, INA219_REG_CALIB, INA219_GetCalibrationValue());
    if (ret != HAL_OK) return ret;

    ret = INA219_ReadRegister(hi2c, addr7, INA219_REG_POWER, &raw);
    if (ret != HAL_OK) return ret;

    *power = (float)raw * (INA219_CURRENT_LSB_A * 20.0f);
    return HAL_OK;
}
