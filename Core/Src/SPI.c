#include "SPI.h"
#include <stdio.h>

/* Use SPI handle from main */
extern SPI_HandleTypeDef hspi1;

/* Chip Select */
#define ADXL_CS_PORT GPIOB
#define ADXL_CS_PIN  GPIO_PIN_12


static int16_t x_off = 0;
static int16_t y_off = 0;
static int16_t z_off = 0;

/* ========= WRITE REGISTER ========= */
static void ADXL_WriteReg(uint8_t reg, uint8_t value)
{
    uint8_t data[2];
    data[0] = reg & 0x3F;
    data[1] = value;

    HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_RESET);
//    HAL_Delay(1);
    HAL_SPI_Transmit(&hspi1, data, 2, 100);
    HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_SET);
}

/* ========= READ MULTI ========= */
static void ADXL_ReadMulti(uint8_t reg, uint8_t *buffer, uint8_t length)
{
    uint8_t tx[7] = {0};
    uint8_t rx[7] = {0};

    tx[0] = reg | 0xC0;   // Read + Multi-byte

    HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_RESET);

    HAL_SPI_TransmitReceive(&hspi1, tx, rx, length + 1, HAL_MAX_DELAY);

    HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_SET);

    /* Skip first dummy byte */
    for (int i = 0; i < length; i++)
    {
        buffer[i] = rx[i + 1];
    }
}

/* ========= INIT ========= */
uint8_t SPI_ADXL_Init(void)
{
    /* Configure ADXL345 */
    ADXL_WriteReg(0x31, 0x01);   // ±4g
    ADXL_WriteReg(0x2D, 0x08);   // Measurement mode

    HAL_Delay(10);

    /* Read Device ID */
    uint8_t tx[2];
    uint8_t rx[2];

    tx[0] = 0x80;   // Read command
    tx[1] = 0x00;

    HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_RESET);
    HAL_SPI_TransmitReceive(&hspi1, tx, rx, 2, 100);
    HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_SET);

    uint8_t device_id = rx[1];

    if (device_id == 0xE5)
    {
        printf("ADXL345 Detected (ID = 0x%02X)\r\n", device_id);
        return 1;   // ✅ success
    }
    else
    {
        printf("ERROR: No device or wrong device! ID = 0x%02X\r\n", device_id);
        return 0;   // ❌ failure
    }
}
/* ========= READ XYZ ========= */
void SPI_ADXL_ReadXYZ(int16_t *x, int16_t *y, int16_t *z)
{
    uint8_t buffer[6];

    ADXL_ReadMulti(0x32, buffer, 6);

    *x = (int16_t)((buffer[1] << 8) | buffer[0]);
    *y = (int16_t)((buffer[3] << 8) | buffer[2]);
    *z = (int16_t)((buffer[5] << 8) | buffer[4]);

//    HAL_Delay(300);
}

void SPI_ADXL_Calibrate(void)
{
    int32_t x_sum = 0, y_sum = 0, z_sum = 0;

    for(int i = 0; i < 50; i++)
    {
        int16_t x, y, z;

        SPI_ADXL_ReadXYZ(&x, &y, &z);

        x_sum += x;
        y_sum += y;
        z_sum += z;

        HAL_Delay(10);
    }

    x_off = x_sum / 50;
    y_off = y_sum / 50;
    z_off = z_sum / 50;
}

//void SPI_ADXL_ReadXYZ_Calibrated(int16_t *x, int16_t *y, int16_t *z)
//{
//    int16_t raw_x, raw_y, raw_z;
//
//    SPI_ADXL_Init();
//    HAL_Delay(100);
//  //
//    SPI_ADXL_Calibrate();
//
//    SPI_ADXL_ReadXYZ(&raw_x, &raw_y, &raw_z);
//
//    *x = raw_x - x_off;
//    *y = raw_y - y_off;
//    *z = raw_z - z_off;
//
//}

void SPI_ADXL_ReadXYZ_Calibrated(int16_t *x, int16_t *y, int16_t *z)
{
    int16_t raw_x, raw_y, raw_z;

    SPI_ADXL_ReadXYZ(&raw_x, &raw_y, &raw_z);

    *x = raw_x - x_off;
    *y = raw_y - y_off;
    *z = raw_z - z_off;
}

void SPI_Run(void)
{
    int16_t x, y, z;

    SPI_ADXL_ReadXYZ_Calibrated(&x, &y, &z);

    printf("X: %d  Y: %d  Z: %d\r\n", x, y, z);

    HAL_Delay(300);
}
