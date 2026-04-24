#include "mpr121.h"
#include <stdio.h>

/* Static handles */
static I2C_HandleTypeDef *mpr_i2c;
static UART_HandleTypeDef *mpr_uart;

/* Keypad mapping */
static const char keyMap[4][4] =
{
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
};

/* Low-level write */
static void MPR121_Write(uint8_t reg, uint8_t data)
{
    HAL_I2C_Mem_Write(mpr_i2c, MPR121_I2C_ADDR,
                      reg, I2C_MEMADD_SIZE_8BIT,
                      &data, 1, 100);
}

/* Read touch */
static uint16_t MPR121_ReadTouch(void)
{
    uint8_t data[2] = {0};

    HAL_I2C_Mem_Read(mpr_i2c, MPR121_I2C_ADDR,
                     0x00, I2C_MEMADD_SIZE_8BIT,
                     data, 2, 100);

    return (data[1] << 8) | data[0];
}

/* Send key */
static void Send_Key_UART(char key)
{
    HAL_UART_Transmit(mpr_uart, (uint8_t *)&key, 1, 100);
    HAL_UART_Transmit(mpr_uart, (uint8_t *)"\r\n", 2, 100);
}

/* Init */
void MPR121_Init(I2C_HandleTypeDef *hi2c, UART_HandleTypeDef *huart)
{
    mpr_i2c = hi2c;
    mpr_uart = huart;

    HAL_Delay(100);

    /* Stop */
    MPR121_Write(0x5E, 0x00);

    /* Thresholds */
    for (uint8_t i = 0; i < 12; i++)
    {
        MPR121_Write(0x41 + i * 2, 12);
        MPR121_Write(0x42 + i * 2, 12);
    }

    /* Start */
    MPR121_Write(0x5E, 0x8C);

//    printf("MPR121 Initialized\r\n");
}

/* Polling function */
void Process_MPR121_Touch(void)
{
    static uint16_t lastTouch = 0;

    uint16_t touch = MPR121_ReadTouch();

    if (touch != 0 && touch != lastTouch)
    {
        int row = -1;
        int col = -1;

        /* Rows */
        for (int r = 0; r < 4; r++)
        {
            if (touch & (1 << r))
            {
                row = r;
                break;
            }
        }

        /* Columns */
        for (int c = 0; c < 4; c++)
        {
            if (touch & (1 << (c + 4)))
            {
                col = c;
                break;
            }
        }

        if (row != -1 && col != -1)
        {
            Send_Key_UART(keyMap[row][col]);
        }
    }

    lastTouch = touch;
}

void MPR121_InterruptHandler(void)
{
    uint16_t touch_status = MPR121_ReadTouch();

    for (int i = 0; i < 12; i++)
    {
        if (touch_status & (1 << i))
        {
            printf("Touch detected on electrode %d\r\n", i);
        }
    }
}

//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
//{
//	if (GPIO_Pin == GPIO_PIN_2)
//	{
//		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
//		Process_MPR121_Touch();
//		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
//		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
//
//
//	}
//}
