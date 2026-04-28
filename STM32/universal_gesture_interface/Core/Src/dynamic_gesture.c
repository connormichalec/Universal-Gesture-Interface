/*
 * dynamic_gesture.c
 *
 *  Created on: Apr 27, 2026
 *      Author: kobes
 */


#include "dynamic_gesture.h"

uint8_t should_trigger_spike_event(SpikeTrigger *trigger, float update_amt) {
	trigger->accum += update_amt;

	if (trigger->accum > trigger->threshold) {
		trigger->accum = 0;
		return 1;
	}

	trigger->accum *= trigger->decay;

	return 0;
}


SpikeTrigger NewSpikeTrigger(float threshold, float decay) {
	SpikeTrigger out;
	out.accum = 0;
	out.threshold = threshold;
	out.decay = decay;

	return out;
}
