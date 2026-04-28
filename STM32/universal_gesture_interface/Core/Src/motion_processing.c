/*
 * motion_processing.c
 *
 *  Created on: Apr 1, 2026
 *      Author: kobes
 */


#include "motion_processing.h"
#include "utils.h"
#include "math.h"

#define DIM 3

// Rotates incoming data (Because our IMU is rotated 45 degrees)
void RotateData(float* x, float* y) {
	float temp_x = *x;
	*x = 0.70710678119 * (*x) + 0.70710678119 * (*y);
	*y = -0.70710678119 * temp_x + 0.70710678119 * (*y);
}

////// IMU PROCESSING ///////

void AccelLPF(float* raw_data, float* accel, float alpha) {
	accel[0] = alpha * raw_data[0] + (1 - alpha) * accel[0];
	accel[1] = alpha * raw_data[1] + (1 - alpha) * accel[1];
	accel[2] = alpha * raw_data[2] + (1 - alpha) * accel[2];
}

// First step: Filter accel data to try to remove gravity component
//	- accel: Raw accelerometer data
//	- prev: Previous raw accelerometer value
//	- hp: Previous calclulated HPF value. Output stored in this array
void HPF(float* accel, float* prev, float* hp, float alpha) {
	for (uint8_t dim = 0; dim < DIM; dim++) {
		float diff = accel[dim] - prev[dim];

		hp[dim] = (int16_t)(alpha * hp[dim] + diff);
		prev[dim] = accel[dim];
	}
}

// Second step: Convert accel data to velocity
//	- accel: Filtered accelerometer data from HPF
//	- vel: Velocity data, output stored in this
//	- drag: Drag coeffecient, meant to counteract drift
void AccelToVel(float* accel, float* vel, float drag, float dt) {
	for (uint8_t dim = 0; dim < 3; dim++) {
		vel[dim] = drag * vel[dim] + accel[dim] * dt;
	}
}

void VelLPF(float* vel, float* lp_vel, float alpha) {
	for (uint8_t dim = 0; dim < DIM; dim++) {
		lp_vel[dim] = alpha * vel[dim] + (1 - alpha) * lp_vel[dim];
	}
}

// Rotates gravity vector then normalizes it
void RotateGravity(float* grav, float* gyro, float dt) {
	float rotated_grav[3];
	cross3(gyro, grav, rotated_grav);
	rotated_grav[0] *= dt;
	rotated_grav[1] *= dt;
	rotated_grav[2] *= dt;

	grav[0] += rotated_grav[0];
	grav[1] += rotated_grav[1];
	grav[2] += rotated_grav[2];
}

// Pulls gravity gradually towards acceleration vector, then renormalizes it
void StabilizeGravity(float* grav, float* accel, float alpha) {
	// Determine magnitude of current acceleration vector (falloff if there's a lot of motion currently)
	float accel_mag_squared = accel[0] * accel[0] + accel[1] * accel[1] + accel[2] * accel[2];

	// Take weighted average if there isn't currently a large motion
	if (accel_mag_squared > 0.64 && accel_mag_squared < 1.44) {		// Magnitude in range 0.8-1.2
		for (uint8_t dim = 0; dim < DIM; dim++) {
			grav[dim] = (1 - alpha) * grav[dim] + alpha * accel[dim];
		}
	}

	// Renormalize
	float grav_mag = sqrt(grav[0] * grav[0] + grav[1] * grav[1] + grav[2] * grav[2]);
	grav[0] /= grav_mag;
	grav[1] /= grav_mag;
	grav[2] /= grav_mag;
}
