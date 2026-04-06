/*
 * flex_sensor.h
 *
 *  Created on: Apr 6, 2026
 *      Author: kobes
 */

#ifndef INC_FLEX_SENSOR_H_
#define INC_FLEX_SENSOR_H_

#include "stm32f405xx.h"

void FlexLPF(uint32_t* flex_reg, uint32_t* lp_reg, float alpha);

#endif /* INC_FLEX_SENSOR_H_ */
