/*
 * LED.c
 *
 *  Created on: 18-Mar-2026
 *      Author: chandan
 */


#include "main.h"
#include "LED.h"

/* LED ON */
void LED_On(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
}

/* LED OFF */
void LED_Off(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
}

/* LED BLINK TASK */
void LED_Toggle_Task(void)
{
//	while(1){
	LED_On();
	HAL_Delay(1000);

	LED_Off();
	HAL_Delay(1000);

//	}

}

/* Optional Init (if needed later) */
void LED_Init(void)
{
    // Not required now since CubeMX already initializes GPIO
}
