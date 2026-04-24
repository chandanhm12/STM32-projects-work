/*
 * i2c_gpio.c
 *
 *  Created on: 24-Mar-2026
 *      Author: chandan
 */


#include "i2c_gpio.h"
#include <string.h>
#include <stdio.h>

extern UART_HandleTypeDef huart3;

/* ================= GPIO CONTROL ================= */

#define SDA_READ()  HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_10)

static void SDA_INPUT(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

static void SDA_OUTPUT(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

static void SCL_HIGH(void) { HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_SET); }
static void SCL_LOW(void)  { HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET); }

static void SDA_HIGH(void) { HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_SET); }
static void SDA_LOW(void)  { HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_RESET); }

/* ================= I2C CORE ================= */

static void I2C_Start(void)
{
    SDA_OUTPUT();
    SDA_HIGH();
    SCL_HIGH();
    HAL_Delay(1);

    SDA_LOW();
    HAL_Delay(1);
    SCL_LOW();
}

static void I2C_Stop(void)
{
    SDA_OUTPUT();
    SDA_LOW();
    HAL_Delay(1);

    SCL_HIGH();
    HAL_Delay(1);

    SDA_HIGH();
    HAL_Delay(1);
}

static void I2C_WriteBit(uint8_t bit)
{
    SDA_OUTPUT();

    (bit) ? SDA_HIGH() : SDA_LOW();

    HAL_Delay(1);
    SCL_HIGH();
    HAL_Delay(1);
    SCL_LOW();
}

static uint8_t I2C_ReadBit(void)
{
    uint8_t bit;

    SDA_INPUT();
    HAL_Delay(1);

    SCL_HIGH();
    HAL_Delay(1);
    bit = SDA_READ();
    SCL_LOW();

    return bit;
}

static uint8_t I2C_WriteByte(uint8_t data)
{
    for (int i = 0; i < 8; i++)
    {
        I2C_WriteBit((data & 0x80) != 0);
        data <<= 1;
    }

    SDA_INPUT();
    HAL_Delay(1);
    SCL_HIGH();
    HAL_Delay(1);

    uint8_t ack = SDA_READ();

    SCL_LOW();
    SDA_OUTPUT();

    return ack;
}

static uint8_t I2C_ReadByte(uint8_t nack)
{
    uint8_t data = 0;

    for (int i = 0; i < 8; i++)
    {
        data <<= 1;
        data |= I2C_ReadBit();
    }

    I2C_WriteBit(nack);
    return data;
}

/* ================= EEPROM ================= */

static uint8_t I2C_SendByte_CheckAck(uint8_t byte)
{
    uint8_t ack = I2C_WriteByte(byte);

    if (ack != 0)
    {
        I2C_Stop();
        char msg[] = "I2C NACK\r\n";
        HAL_UART_Transmit(&huart3, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
        return 1;
    }
    return 0;
}

static void EEPROM_WriteByte(uint16_t addr, uint8_t data)
{
    I2C_Start();

    if (I2C_SendByte_CheckAck(0xA0)) return;
    if (I2C_SendByte_CheckAck(addr >> 8)) return;
    if (I2C_SendByte_CheckAck(addr & 0xFF)) return;
    if (I2C_SendByte_CheckAck(data)) return;

    I2C_Stop();
    HAL_Delay(5);
}

static uint8_t EEPROM_ReadByte(uint16_t addr)
{
    uint8_t data;

    I2C_Start();
    if (I2C_SendByte_CheckAck(0xA0)) return 0;

    if (I2C_SendByte_CheckAck(addr >> 8)) return 0;
    if (I2C_SendByte_CheckAck(addr & 0xFF)) return 0;

    I2C_Start();
    if (I2C_SendByte_CheckAck(0xA1)) return 0;

    data = I2C_ReadByte(1);
    I2C_Stop();

    return data;
}

/* ================= PUBLIC FUNCTIONS ================= */

void I2C_GPIO_InitBus(void)
{
    SDA_INPUT();
    SCL_HIGH();
    HAL_Delay(1);
}

void I2C_GPIO_RunTest(void)
{
    char msg[64];

    EEPROM_WriteByte(0x0000, 0x54);
    HAL_Delay(10);

    uint8_t value = EEPROM_ReadByte(0x0000);

    snprintf(msg, sizeof(msg), "Data = 0x%02X\r\n", value);
    HAL_UART_Transmit(&huart3, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
}
