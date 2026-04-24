/*
 * SPI.h
 *
 *  Created on: 18-Mar-2026
 *      Author: chandan
 */

#ifndef INC_SPI_H_
#define INC_SPI_H_

#include "main.h"     //IMPORTANT (for HAL types like int16_t, GPIO, etc.)
#include <stdint.h>

/* Function Prototypes */
//void SPI_ADXL_Init(void);
uint8_t SPI_ADXL_Init(void);
void SPI_ADXL_Calibrate(void);
void SPI_ADXL_ReadXYZ(int16_t *x, int16_t *y, int16_t *z);
void SPI_ADXL_ReadXYZ_Calibrated(int16_t *x, int16_t *y, int16_t *z);
void SPI_Run(void);

#endif /* INC_SPI_H_ */
