/*
 * Uart.h
 *
 *  Created on: 24-Mar-2026
 *      Author: chandan
 */

#ifndef UART_H
#define UART_H

#include "main.h"

// Modes
#define MODE_MENU 0
#define MODE_LED  1
#define MODE_SPI  2
#define MODE_EEPROM 3
#define MODE_GPIO_I2C 4
#define MODE_PWM  5
#define MODE_PWM_STOP 6
#define MODE_ds18b20 7
#define MODE_MPR121   8

void UART_Menu_Init(UART_HandleTypeDef *huart);
uint8_t UART_GetMode(void);

#endif
