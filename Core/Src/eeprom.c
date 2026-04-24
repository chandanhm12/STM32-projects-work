/*
 * eeprom.c
 *
 *  Created on: 24-Mar-2026
 *      Author: chandan
 */

#include "eeprom.h"
#include <stdio.h>
#include <string.h>

static I2C_HandleTypeDef *eeprom_i2c;
static UART_HandleTypeDef *eeprom_uart;

#define EEPROM_WRITE_ADDR 0xA0
#define EEPROM_READ_ADDR  0xA1

void EEPROM_Init(I2C_HandleTypeDef *hi2c, UART_HandleTypeDef *huart)
{
    eeprom_i2c = hi2c;
    eeprom_uart = huart;
}

uint8_t EEPROM_IsConnected(void)
{
    if (HAL_I2C_IsDeviceReady(eeprom_i2c, EEPROM_WRITE_ADDR, 3, 100) == HAL_OK)
    {
        return 1;   // ✅ device present
    }
    else
    {
        return 0;   // ❌ not present
    }
}

void EEPROM_WriteFullMemory(void)
{
    uint8_t test_data = 0xAA;
    uint8_t eeprom_page[64];
    char msg[80];

    /* ===== STEP 1: CHECK DEVICE ===== */
//    if (!EEPROM_IsConnected())
//    {
//        sprintf(msg, "EEPROM NOT CONNECTED!\r\n");
//        HAL_UART_Transmit(eeprom_uart, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
//        return;
//    }

    sprintf(msg, "EEPROM DETECTED, STARTING WRITE...\r\n");
    HAL_UART_Transmit(eeprom_uart, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

    HAL_Delay(10);

    /* ===== STEP 2: FULL MEMORY WRITE ===== */
    for (uint16_t page = 0; page < 512; page++)
    {
        uint16_t mem_addr = page * 64;

        for (uint8_t i = 0; i < 64; i++)
        {
            eeprom_page[i] = (uint8_t)(mem_addr + i);
        }

        if (HAL_I2C_Mem_Write(eeprom_i2c, EEPROM_WRITE_ADDR,
                              mem_addr, I2C_MEMADD_SIZE_16BIT,
                              eeprom_page, 64, HAL_MAX_DELAY) != HAL_OK)
        {
            /* ❌ If ANY write fails → treat as no device */
            sprintf(msg, "EEPROM NOT CONNECTED! WRITE FAILED at page %u\r\n", page);
            HAL_UART_Transmit(eeprom_uart, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

            return;   // 🔥 STOP immediately
        }

        sprintf(msg, "Page %u WRITE OK\r\n", page);
        HAL_UART_Transmit(eeprom_uart, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

        HAL_Delay(10);
    }

    HAL_UART_Transmit(eeprom_uart, (uint8_t*)"WRITE COMPLETE\r\n", 16, HAL_MAX_DELAY);
}


void EEPROM_ReadFullMemory(void)
{
    uint8_t buffer[64];
    char msg[100];

//    if (!EEPROM_IsConnected())
//    {
//        sprintf(msg, "EEPROM NOT CONNECTED!\r\n");
//        HAL_UART_Transmit(eeprom_uart, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
//        return;
//    }

    for (uint16_t page = 0; page < 512; page++)
    {
        uint16_t mem_addr = page * 64;

        if (HAL_I2C_Mem_Read(eeprom_i2c, EEPROM_READ_ADDR,
                            mem_addr, I2C_MEMADD_SIZE_16BIT,
                            buffer, 64, HAL_MAX_DELAY) == HAL_OK)
        {
            snprintf(msg, sizeof(msg), "\r\nPAGE %u\r\n", page);
            HAL_UART_Transmit(eeprom_uart, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

            for (int i = 0; i < 64; i++)
            {
                snprintf(msg, sizeof(msg), "%03u ", buffer[i]);
                HAL_UART_Transmit(eeprom_uart, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
            }

            HAL_UART_Transmit(eeprom_uart, (uint8_t*)"\r\n", 2, HAL_MAX_DELAY);
        }

        HAL_Delay(2);
    }

    HAL_UART_Transmit(eeprom_uart, (uint8_t*)"READ COMPLETE\r\n", 16, HAL_MAX_DELAY);
}


void EEPROM_VerifyFullMemory(void)
{
    uint8_t read_buffer[64];
    char msg[120];

    if (!EEPROM_IsConnected())
    {
        sprintf(msg, "EEPROM NOT CONNECTED!\r\n");
        HAL_UART_Transmit(eeprom_uart, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
        return;
    }

    for (uint16_t page = 0; page < 512; page++)
    {
        uint16_t mem_addr = page * 64;

        if (HAL_I2C_Mem_Read(eeprom_i2c, EEPROM_READ_ADDR,
                            mem_addr, I2C_MEMADD_SIZE_16BIT,
                            read_buffer, 64, HAL_MAX_DELAY) != HAL_OK)
        {
            snprintf(msg, sizeof(msg), "Page %u READ ERROR\r\n", page);
            HAL_UART_Transmit(eeprom_uart, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
            continue;
        }

        uint8_t match = 1;

        for (uint8_t i = 0; i < 64; i++)
        {
            uint8_t expected = mem_addr + i;

            if (read_buffer[i] != expected)
            {
                match = 0;

                uint32_t abs_addr = mem_addr + i;

                snprintf(msg, sizeof(msg),
                "Page %u MISMATCH at addr %lu: exp=%u read=%u\r\n",
                page, (unsigned long)abs_addr, expected, read_buffer[i]);

                HAL_UART_Transmit(eeprom_uart, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
                break;
            }
        }

        if (match)
        {
            snprintf(msg, sizeof(msg), "Page %u VERIFY OK\r\n", page);
            HAL_UART_Transmit(eeprom_uart, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
        }

        HAL_Delay(2);
    }

    HAL_UART_Transmit(eeprom_uart, (uint8_t*)"VERIFY COMPLETE\r\n", 18, HAL_MAX_DELAY);
}


