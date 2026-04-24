#include "pwm_Timer.h"
#include "main.h"
#include <stdio.h>

/* External handles from main */
extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim10;

/* Internal variables */
static uint32_t time_ms = 0;
static uint8_t pwm_running = 0;

/* Helper */
static void All_LED_Off(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
}

/* Init */
void PWM_Init(void)
{
    HAL_TIM_PWM_Start(&htim10, TIM_CHANNEL_1);
}

/* Start sequence */
void PWM_StartSequence(void)
{
    pwm_running = 1;
    time_ms = 0;
    HAL_TIM_Base_Start_IT(&htim6);
}

/* Stop sequence */
void PWM_StopSequence(void)
{
    pwm_running = 0;
    HAL_TIM_Base_Stop_IT(&htim6);
    All_LED_Off();
}

/* Timer callback */
void PWM_TimerCallback(TIM_HandleTypeDef *htim)
{
    if (!pwm_running) return;

    if (htim->Instance == TIM6)
    {
        time_ms++;

        if (time_ms == 2)
        {
            All_LED_Off();
            __HAL_TIM_SET_COMPARE(&htim10, TIM_CHANNEL_1, 250);
            printf("2 sec -> 25%%\r\n");
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
        }
        else if (time_ms == 4)
        {
            All_LED_Off();
            __HAL_TIM_SET_COMPARE(&htim10, TIM_CHANNEL_1, 500);
            printf("4 sec -> 50%%\r\n");
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
        }
        else if (time_ms == 6)
        {
            All_LED_Off();
            __HAL_TIM_SET_COMPARE(&htim10, TIM_CHANNEL_1, 750);
            printf("6 sec -> 75%%\r\n");
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
        }
        else if (time_ms == 8)
        {
            __HAL_TIM_SET_COMPARE(&htim10, TIM_CHANNEL_1, 999);
            printf("8 sec -> 100%%\r\n");

            All_LED_Off();
            time_ms = 0;
        }
    }
}
