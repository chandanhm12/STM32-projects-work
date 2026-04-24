/*
 * mpr121.h
 *
 *  Created on: 20-Apr-2026
 *      Author: chandu
 */
#ifndef MPR121_H
#define MPR121_H

#include "main.h"
#include <stdint.h>

/* I2C Address */
#define MPR121_I2C_ADDR (0x5A << 1)

/* Function Prototypes */
void MPR121_Init(I2C_HandleTypeDef *hi2c, UART_HandleTypeDef *huart);
void Process_MPR121_Touch(void);
void MPR121_InterruptHandler(void);

#endif
