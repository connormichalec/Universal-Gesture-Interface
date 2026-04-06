/*
 * motion_processing.c
 *
 *  Created on: Apr 1, 2026
 *      Author: kobes
 */


#include "motion_processing.h"

#define DIM 3

////// IMU PROCESSING ///////

// First step: Filter accel data to try to remove gravity component
//	- accel: Raw accelerometer data
//	- prev: Previous raw accelerometer value
//	- hp: Previous calclulated HPF value. Output stored in this array
void HPF(int16_t* accel, int16_t* prev, float* hp, float alpha) {
	for (uint8_t dim = 0; dim < DIM; dim++) {
		// Widening to prevent overflow
		float diff = (float) accel[dim] - (float) prev[dim];

	    hp[dim] = diff;
		//hp[dim] = (int16_t)(alpha * hp[dim] + accel[dim] - prev[dim]);
		prev[dim] = accel[dim];
	}
}

// Second step: Convert accel data to velocity
//	- accel: Filtered accelerometer data from HPF
//	- vel: Velocity data, output stored in this
//	- drag: Drag coeffecient, meant to counteract drift
void AccelToVel(float* accel, float* vel, float drag, float dt) {
	for (uint8_t dim = 0; dim < 3; dim++) {
		vel[dim] = drag * vel[dim] + accel[dim] * dt / 1000.0f;
	}
}

void VelLPF(float* vel, float* lp_vel, float alpha) {
	for (uint8_t dim = 0; dim < DIM; dim++) {
		lp_vel[dim] = alpha * vel[dim] + (1 - alpha) * lp_vel[dim];
	}
}
