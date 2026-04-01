/*
 * motion_processing.c
 *
 *  Created on: Apr 1, 2026
 *      Author: kobes
 */


#include "motion_processing.h"



////// IMU PROCESSING ///////

// First step: Filter accel data to try to remove gravity component
//	- accel: Raw accelerometer data
//	- prev: Previous raw accelerometer value
//	- hp: Previous calclulated HPF value. Output stored in this array
void HPF(int16_t* accel, int16_t* prev, int16_t* hp, float alpha) {
	for (uint8_t dim = 0; dim < 2; dim++) {		// Change to 3 if ever using 3 dimensions
		hp[dim] = alpha * (hp[dim] + accel[dim] - prev[dim]);
		prev[dim] = accel[dim];
	}
}

// Second step: Convert accel data to velocity
//	- accel: Filtered accelerometer data from HPF
//	- vel: Velocity data, output stored in this
//	- drag: Drag coeffecient, meant to counteract drift
void AccelToVel(int16_t* accel, int16_t* vel, float drag, float dt) {
	for (uint8_t dim = 0; dim < 2; dim++) {
		vel[dim] = drag * vel[dim] + accel[dim] * dt;
	}
}
