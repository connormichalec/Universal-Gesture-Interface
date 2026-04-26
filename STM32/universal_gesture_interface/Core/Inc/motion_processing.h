/*
 * motion_processing.h
 *
 *  Created on: Apr 1, 2026
 *      Author: kobes
 */

#include "stm32f4xx_hal.h"

#ifndef INC_MOTION_PROCESSING_H_
#define INC_MOTION_PROCESSING_H_

void RotateData(float* x, float* y);
void HPF(float* accel, float* prev, float* hp, float alpha);
void AccelToVel(float* accel, float* vel, float drag, float dt);
void VelLPF(float* vel, float* lp_vel, float alpha);

#endif /* INC_MOTION_PROCESSING_H_ */
