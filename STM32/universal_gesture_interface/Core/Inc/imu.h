/*
 * imu.h
 *
 *  Created on: Mar 24, 2026
 *      Author: kobes
 */

#include "stm32f4xx_hal.h"


#ifndef INC_IMU_H_
#define INC_IMU_H_

#define CS_GPIO_Port GPIOB
#define CS_Pin GPIO_PIN_6

#define CS_LOW()  HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET)
#define CS_HIGH() HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET)

uint8_t IMU_Init(SPI_HandleTypeDef *hspi);
void IMU_ReadAccel(SPI_HandleTypeDef *hspi, int16_t *accel);
void IMU_ReadGyro(SPI_HandleTypeDef *hspi, int16_t *gyro);

#endif /* INC_IMU_H_ */
