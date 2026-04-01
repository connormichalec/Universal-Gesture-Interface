/*
 * motion_processing.h
 *
 *  Created on: Apr 1, 2026
 *      Author: kobes
 */

#include "stm32f4xx_hal.h"

#ifndef INC_MOTION_PROCESSING_H_
#define INC_MOTION_PROCESSING_H_

void HPF(int16_t* accel, int16_t* prev, int16_t* hp, float alpha);
void AccelToVel(int16_t* accel, int16_t* vel, float drag, float dt);

#endif /* INC_MOTION_PROCESSING_H_ */
