/*
 * flex_sensor.h
 *
 *  Created on: Apr 6, 2026
 *      Author: kobes
 */

#ifndef INC_FLEX_SENSOR_H_
#define INC_FLEX_SENSOR_H_

#include "stm32f405xx.h"

void FlexLPF(float flex_reg, float* lp_reg, float alpha);
float FlexRemap(uint16_t val, int16_t min, int16_t range);

#endif /* INC_FLEX_SENSOR_H_ */
