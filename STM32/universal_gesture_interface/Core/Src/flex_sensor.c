/*
 * flex_sensor.c
 *
 *  Created on: Apr 6, 2026
 *      Author: kobes
 */


#include "flex_sensor.h"

void FlexLPF(uint32_t* flex_reg, uint32_t* lp_reg, float alpha) {
	*lp_reg = alpha * (*flex_reg) + (1.0 - alpha) * (*lp_reg);
}

uint32_t FlexRemap(uint32_t val) {
	return 0;
}
