/*
 * INA219.h
 *
 *  Created on: Jul 30, 2026
 *      Author: geeeethb
 */

#ifndef INA219_H
#define INA219_H

#include "stm32l0xx_hal.h"
#include "stdint.h"

#define INA219_ADDR_0          0x40
#define INA219_ADDR_1          0x41

#define INA219_REG_CONFIG      0x00
#define INA219_REG_SHUNT_V     0x01
#define INA219_REG_BUS_V       0x02
#define INA219_REG_POWER       0x03
#define INA219_REG_CURRENT     0x04
#define INA219_REG_CALIB       0x05

#define INA219_SHUNT_OHMS      0.1f
#define INA219_CURRENT_LSB_A   0.0001f   // 100uA/bit


/* Define of Prototype Functions */
HAL_StatusTypeDef INA219_Init(I2C_HandleTypeDef *hi2c, uint8_t addr7);
HAL_StatusTypeDef INA219_ReadRegister(I2C_HandleTypeDef *hi2c, uint8_t addr7, uint8_t reg, uint16_t *data);
HAL_StatusTypeDef INA219_WriteRegister(I2C_HandleTypeDef *hi2c, uint8_t addr7, uint8_t reg,uint16_t data);
HAL_StatusTypeDef INA219_ReadBusVoltage_V(I2C_HandleTypeDef *hi2c, uint8_t addr7, float *voltage);
HAL_StatusTypeDef INA219_ReadShuntVoltage_mV(I2C_HandleTypeDef *hi2c, uint8_t addr7, float *voltage);
HAL_StatusTypeDef INA219_ReadCurrent_A(I2C_HandleTypeDef *hi2c, uint8_t addr7, float *current);
HAL_StatusTypeDef INA219_ReadPower_W(I2C_HandleTypeDef *hi2c, uint8_t addr7, float *power);

#endif
