/*
 * imu.c
 *
 *  Created on: Mar 24, 2026
 *      Author: kobes
 *
 *  Description: Relevant functions and variables for interfacing with the LSM6DSL IMU
 */

#include "imu.h"


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
