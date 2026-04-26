/*
 * flex_sensor.c
 *
 *  Created on: Apr 6, 2026
 *      Author: kobes
 */


#include "flex_sensor.h"

void FlexLPF(float flex_reg, float* lp_reg, float alpha) {
	*lp_reg = alpha * (flex_reg) + (1.0 - alpha) * (*lp_reg);
}

float FlexRemap(uint16_t val, int16_t min, int16_t range) {
	float out = (float) (val - min) / range;

	if (out > 1.0) return 1.0;
	if (out < 0.0) return 0.0;
	return out;
}
