/*
 * ds18b20.h
 *
 *  Created on: 20-Apr-2026
 *      Author: chandu
 */

#ifndef INC_DS18B20_H_
#define INC_DS18B20_H_

#include "main.h"

/* Initialization */
void DS18B20_Init(void);
extern TIM_HandleTypeDef htim6;
//void DWT_Delay_Init(void);

/* Core functions */
uint8_t DS18B20_Reset(void);
float DS18B20_ReadTemperature(void);



#endif /* INC_DS18B20_H_ */
