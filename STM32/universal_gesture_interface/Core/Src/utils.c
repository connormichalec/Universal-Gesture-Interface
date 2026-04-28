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


void cross3(float a[3], float b[3], float out[3])
{
    float x = a[1]*b[2] - a[2]*b[1];
    float y = a[2]*b[0] - a[0]*b[2];
    float z = a[0]*b[1] - a[1]*b[0];

    out[0] = x;
    out[1] = y;
    out[2] = z;
}
