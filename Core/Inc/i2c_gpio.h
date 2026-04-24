/*
 * i2c_gpio.h
 *
 *  Created on: 24-Mar-2026
 *      Author: chandan
 */

#ifndef I2C_GPIO_H
#define I2C_GPIO_H

#include "stm32f7xx_hal.h"

/* Public APIs */
void I2C_GPIO_InitBus(void);
void I2C_GPIO_RunTest(void);

#endif
