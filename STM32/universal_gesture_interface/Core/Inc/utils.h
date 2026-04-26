/*
 * utils.h
 *
 *  Created on: Apr 24, 2026
 *      Author: kobes
 */

#ifndef INC_UTILS_H_
#define INC_UTILS_H_

#include "stm32f4xx_hal.h"

typedef struct {
	uint32_t period;
	uint32_t last_update;
} PeriodicEventHandler;

uint8_t shouldTick(uint32_t tick, PeriodicEventHandler *handler);
PeriodicEventHandler NewPeriodicEvent(uint32_t period);

#endif /* INC_UTILS_H_ */
