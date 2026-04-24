/*
 * ds18b20.c
 *
 *  Created on: 20-Apr-2026
 *      Author: chandan (fixed version)
 */

#include "ds18b20.h"

/* ================= PIN CONFIG ================= */
#define DS18B20_PORT GPIOA
#define DS18B20_PIN  GPIO_PIN_3

#define DS18B20_PIN_POS 3   // PA3 → position 3

/* ================= DWT DELAY ================= */

void DWT_Delay_Init(void)
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

void delay_us(uint32_t us)
{
    if (!(DWT->CTRL & DWT_CTRL_CYCCNTENA_Msk))
        return;  // prevent infinite loop

    uint32_t cycles = us * (SystemCoreClock / 1000000);
    uint32_t start = DWT->CYCCNT;

    while ((DWT->CYCCNT - start) < cycles);
}


//static void delay_us(uint32_t us)
//{
//    uint32_t count = (SystemCoreClock / 1000000) * us / 5;
//
//    while (count--)
//    {
//        __NOP();
//    }
//}
/* ================= GPIO CONTROL ================= */

//static void DQ_Output(void)
//{
//    /* Set as Output */
//    DS18B20_PORT->MODER &= ~(3 << (DS18B20_PIN_POS * 2));
//    DS18B20_PORT->MODER |=  (1 << (DS18B20_PIN_POS * 2));
//
//    /* Open-drain */
//    DS18B20_PORT->OTYPER |= DS18B20_PIN;
//}

static void DQ_Output(void)
{
    GPIOA->MODER &= ~(3 << (3 * 2));
    GPIOA->MODER |=  (1 << (3 * 2));   // Output mode
}

//static void DQ_Input(void)
//{
//    /* Set as Input (floating, external pull-up used) */
//    DS18B20_PORT->MODER &= ~(3 << (DS18B20_PIN_POS * 2));
//}

static void DQ_Input(void)
{
    GPIOA->MODER &= ~(3 << (3 * 2));   // Input mode
}

//static inline void DQ_Low(void)
//{
//    DQ_Output();
//    DS18B20_PORT->BSRR = (uint32_t)DS18B20_PIN << 16;
//}

static inline void DQ_Low(void)
{
    DQ_Output();
    GPIOA->BSRR = (uint32_t)GPIO_PIN_3 << 16;
}

static inline void DQ_Release(void)
{
    DQ_Input();
}

//static inline uint8_t DQ_Read(void)
//{
//    return (DS18B20_PORT->IDR & DS18B20_PIN) ? 1 : 0;
//}

static inline uint8_t DQ_Read(void)
{
    return (GPIOA->IDR & GPIO_PIN_3) ? 1 : 0;
}


/* ================= CORE ================= */

//void DS18B20_Init(void)
//{
//    DWT_Delay_Init();
//}

uint8_t DS18B20_Reset(void)
{
    uint8_t presence;

    DQ_Low();
    delay_us(480);

    DQ_Release();
    delay_us(70);

    presence = DQ_Read();   // 0 = device present

    delay_us(410);

    return (presence == 0);
}

/* ================= LOW LEVEL ================= */

static void DS18B20_WriteBit(uint8_t bit)
{
    if (bit)
    {
        DQ_Low();
        delay_us(5);
        DQ_Release();
        delay_us(55);
    }
    else
    {
        DQ_Low();
        delay_us(60);
        DQ_Release();
        delay_us(5);
    }
}

static uint8_t DS18B20_ReadBit(void)
{
    uint8_t bit;

    DQ_Low();
    delay_us(3);

    DQ_Release();
    delay_us(10);

    bit = DQ_Read();

    delay_us(50);

    return bit;
}

static void DS18B20_WriteByte(uint8_t data)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        DS18B20_WriteBit(data & 0x01);
        data >>= 1;
    }
}

static uint8_t DS18B20_ReadByte(void)
{
    uint8_t data = 0;

    for (uint8_t i = 0; i < 8; i++)
    {
        data |= (DS18B20_ReadBit() << i);
    }

    return data;
}

/* ================= USER FUNCTION ================= */

float DS18B20_ReadTemperature(void)
{
    uint8_t tempL, tempH;
    int16_t raw;

    /* Start conversion */
    if (!DS18B20_Reset())
        return -1000.0f;

    DS18B20_WriteByte(0xCC);   // Skip ROM
    DS18B20_WriteByte(0x44);   // Convert T

    HAL_Delay(750);           // Wait conversion

    /* Read scratchpad */
    if (!DS18B20_Reset())
        return -1000.0f;

    DS18B20_WriteByte(0xCC);
    DS18B20_WriteByte(0xBE);

    tempL = DS18B20_ReadByte();
    tempH = DS18B20_ReadByte();

    raw = (tempH << 8) | tempL;

    return (float)raw / 16.0f;
}
