/*
 * imu.c
 *
 *  Created on: Mar 24, 2026
 *      Author: kobes
 *
 *  Description: Relevant functions and variables for interfacing with the LSM6DSL IMU
 */

#include "imu.h"

extern SPI_HandleTypeDef hspi1;

// LSM6DSL registers
#define WHO_AM_I        0x0F
#define CTRL1_XL        0x10
#define CTRL2_G         0x11
#define CTRL3_C         0x12

// SPI read/write helpers
static uint8_t LSM6DSL_ReadReg(uint8_t reg)
{
    uint8_t tx[2], rx[2];
    tx[0] = 0x80 | reg;   // MSB=1 for read
    tx[1] = 0x00;         // dummy byte

    CS_LOW();
    HAL_SPI_TransmitReceive(&hspi1, tx, rx, 2, HAL_MAX_DELAY);
    CS_HIGH();

    return rx[1];
}

static void LSM6DSL_WriteReg(uint8_t reg, uint8_t value)
{
    uint8_t tx[2];
    tx[0] = reg & 0x7F; // MSB=0 for write
    tx[1] = value;

    CS_LOW();
    HAL_SPI_Transmit(&hspi1, tx, 2, HAL_MAX_DELAY);
    CS_HIGH();
}

uint8_t IMU_Init(SPI_HandleTypeDef *hspi)
{
    uint8_t whoami;

    // Ensure CS is HIGH at init
    CS_HIGH();
    HAL_Delay(10); // let IMU power up

    // Check WHO_AM_I
    whoami = LSM6DSL_ReadReg(WHO_AM_I);
    if(whoami != 0x6A)   // LSM6DSL WHO_AM_I = 0x6A
    {
        return 0; // error
    }

    // Reset device
    LSM6DSL_WriteReg(CTRL3_C, 0x01); // SW_RESET
    HAL_Delay(50);

    // Enable Block Data Update (optional, recommended)
    LSM6DSL_WriteReg(CTRL3_C, 0x44); // BDU=1, IF_INC=1

    // Configure accelerometer: 104 Hz, ±4g
    // ODR_XL = 104 Hz, FS_XL = ±4g, BW = 100 Hz
    LSM6DSL_WriteReg(CTRL1_XL, 0x48);

    // Configure gyroscope: 104 Hz, 2000 dps
    LSM6DSL_WriteReg(CTRL2_G, 0x4C);

    return 1; // success
}

void IMU_ReadAccel(SPI_HandleTypeDef *hspi, int16_t *accel)
{
    uint8_t tx[7];
    uint8_t rx[7];

    tx[0] = 0x28 | 0x80 | 0x40;  // Read + auto-increment
    for (int i = 1; i < 7; i++) {
        tx[i] = 0x00; // dummy bytes
    }

    CS_LOW();

    HAL_SPI_TransmitReceive(hspi, tx, rx, 7, HAL_MAX_DELAY);

    CS_HIGH();

    // Combine bytes (little endian)
    accel[0] = (int16_t)(rx[2] << 8 | rx[1]); // X
    accel[1] = (int16_t)(rx[4] << 8 | rx[3]); // Y
    accel[2] = (int16_t)(rx[6] << 8 | rx[5]); // Z
}

void IMU_ReadGyro(SPI_HandleTypeDef *hspi, int16_t *gyro)
{
    uint8_t tx[7];
    uint8_t rx[7];

    // Read + auto-increment starting at OUTX_L_G (0x22)
    tx[0] = 0x22 | 0x80 | 0x40;

    for (int i = 1; i < 7; i++) {
        tx[i] = 0x00; // dummy bytes
    }

    CS_LOW();

    HAL_SPI_TransmitReceive(hspi, tx, rx, 7, HAL_MAX_DELAY);

    CS_HIGH();

    // Combine low/high bytes (little-endian)
    gyro[0] = (int16_t)(rx[2] << 8 | rx[1]); // X
    gyro[1] = (int16_t)(rx[4] << 8 | rx[3]); // Y
    gyro[2] = (int16_t)(rx[6] << 8 | rx[5]); // Z
}
