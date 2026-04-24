/*
 * Uart.c
 *
 *  Created on: 24-Mar-2026
 *      Author: chandan
 */


#include "uart.h"
#include <stdio.h>

static UART_HandleTypeDef *uart_handle;
static uint8_t rx_data;
static volatile uint8_t mode = MODE_MENU;

void UART_Menu_Init(UART_HandleTypeDef *huart)
{
    uart_handle = huart;

    printf("\r\n==== MENU ====\r\n");
//    printf("\r\n==== MENU ====\r\n");
    printf("Press 1 -> LED Program\r\n");
    printf("Press 2 -> SPI Program\r\n");
    printf("Press 3 -> EEPROM Test\r\n");
    printf("Press 4 -> EEPROM Gpio Test\r\n");
    printf("Press 5 -> PWM Start mode\r\n");
    printf("Press 6 -> PWM stop condition\r\n");
    printf("Press 7 -> Temprature sensor ds18b20 readings...\r\n");
    printf("Press 8 -> MPR121 TOUCH Sensor Mode selected....... \r\n");


    HAL_UART_Receive_IT(uart_handle, &rx_data, 1);
}

uint8_t UART_GetMode(void)
{
    return mode;
}

// Interrupt Callback
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == uart_handle)
    {
        if (rx_data == '1')
        {
            mode = MODE_LED;
            printf("LED Mode Selected\r\n");
        }
        else if (rx_data == '2')
        {
            mode = MODE_SPI;
            printf("SPI Mode Selected\r\n");
        }
        else if (rx_data == '3')
        {
            mode = MODE_EEPROM;
            printf("EEPROM Mode Selected\r\n");
        }

        else if (rx_data == '4')
        {
        	mode = MODE_GPIO_I2C;
        	printf("EEprom I2C Gpio mode selected \r\n");
        }

        else if (rx_data == '5')
        {
        	mode = MODE_PWM;
        	printf("PWM Timer Inrerrupt Mode stoped \r\n");
        }

        else if (rx_data == '6')
        {
        	mode = MODE_PWM_STOP;
        	printf("PWM Timer Inrerrupt Mode selected \r\n");
        }

        else if (rx_data == '7')
        {
        	mode = MODE_ds18b20;
        	printf("ds18b20 temprature sensor 1 line read write selected.. Mode selected \r\n");
        }
        else if (rx_data == '8')
        {
        	mode = MODE_MPR121;
        	printf("MPR121 TOUCH Sensor Mode selected....... \r\n");
        }


        else
        {
            mode = MODE_MENU;
            printf("\r\n==== MENU ====\r\n");
            printf("Press 1 -> LED Program\r\n");
            printf("Press 2 -> SPI Program\r\n");
            printf("Press 3 -> EEPROM Test\r\n");
            printf("Press 4 -> EEPROM Gpio Test\r\n");
            printf("Press 5 -> PWM Start mode\r\n");
            printf("Press 6 -> PWM stop condition\r\n");
            printf("Press 7 -> Temprature sensor ds18b20 readings...\r\n");
        }

        HAL_UART_Receive_IT(uart_handle, &rx_data, 1);
    }
}
