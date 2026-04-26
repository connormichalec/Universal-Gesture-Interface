/*
 * utils.c
 *
 *  Created on: Apr 24, 2026
 *      Author: kobes
 */


#include "utils.h"

// Takes a event handler and returns true when an event should be triggered
uint8_t shouldTick(uint32_t tick, PeriodicEventHandler *handler) {
	if (tick > handler->last_update + handler->period) {
		handler->last_update += handler->period;
		return 1;
	}

	return 0;
}


// Just an easier syntax for quickly making a new periodic event handler
PeriodicEventHandler NewPeriodicEvent(uint32_t period) {	// Period in ms
	PeriodicEventHandler out;
	out.last_update = HAL_GetTick();
	out.period = period;

	return out;
}
