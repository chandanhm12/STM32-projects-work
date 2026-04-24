/*
 * eeprom.h
 *
 *  Created on: 24-Mar-2026
 *      Author: chandan
 */
#ifndef EEPROM_H
#define EEPROM_H

#include "main.h"

uint8_t EEPROM_IsConnected(void);
void EEPROM_Init(I2C_HandleTypeDef *hi2c, UART_HandleTypeDef *huart);

void EEPROM_WriteFullMemory(void);
void EEPROM_ReadFullMemory(void);
void EEPROM_VerifyFullMemory(void);

#endif
