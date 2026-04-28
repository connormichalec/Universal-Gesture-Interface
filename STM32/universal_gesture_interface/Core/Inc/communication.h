/*
 * comunication.h
 *
 *  Created on: Apr 27, 2026
 *      Author: kobes
 */

#ifndef INC_COMMUNICATION_H_
#define INC_COMMUNICATION_H_

#include "stm32f4xx_hal.h"

#pragma pack (1)

typedef struct __attribute__((packed)) {
	uint16_t topic;	// Header
	float accel_x;
	float accel_y;
	float accel_z;
	float grav_x;
	float grav_y;
	float grav_z;
	float gyro_x;
	float gyro_y;
	float gyro_z;
	float flex_thumb;
	float flex_index;
	float flex_middle;
	uint16_t fsr_thumb;
	uint16_t fsr_index;
	uint16_t fsr_middle;
} SensorUpdatePacket;

typedef struct __attribute__((packed)) {
	uint16_t topic; // Header
	uint16_t gesture_id;
} GesturePacket;

#endif /* INC_COMMUNICATION_H_ */
