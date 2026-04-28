/*
 * dynamic_gesture.h
 *
 *  Created on: Apr 27, 2026
 *      Author: kobes
 */

#ifndef INC_DYNAMIC_GESTURE_H_
#define INC_DYNAMIC_GESTURE_H_

#include "stm32f4xx_hal.h"


// Used for events that correspond to a spike in a value
typedef struct {
	float accum;
	float decay;
	float threshold;
} SpikeTrigger;

uint8_t should_trigger_spike_event(SpikeTrigger *trigger, float update_amt);
SpikeTrigger NewSpikeTrigger(float threshold, float decay);


#endif /* INC_DYNAMIC_GESTURE_H_ */
