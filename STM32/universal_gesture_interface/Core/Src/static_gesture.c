/*
 * static_gesture.c
 *
 *  Created on: Apr 25, 2026
 *      Author: kobes
 */

#include "static_gesture.h"

#define MAX_ACCUMULATION 25
#define TRIGGER_THRESHOLD 15
#define TRIGGER_DEACTIVATION_THRESHOLD 4

extern float accel[3];
extern float gyro[3];
extern float vel[3];
extern float grav[3];
extern float flex_thumb;
extern float flex_index;
extern float flex_middle;
extern StaticGesture current_gesture;

StaticEventHandler NewStaticEventHandler() {
	StaticEventHandler out;
	out.accum = 0;
	out.gesture_triggered = 0;
	return out;
}

// Returns true when a corresponding event should be triggered
uint8_t should_trigger_gesture(StaticEventHandler *handler, uint8_t gesture_active) {
	// Accumulator handling
	if (gesture_active) {
		if (handler->accum < MAX_ACCUMULATION) handler->accum++;
	}
	else {
		if (handler->accum != 0) handler->accum--;
	}

	// When crossing threshold and trigger is not available, activate trigger
	if (handler->accum > TRIGGER_THRESHOLD && !handler->gesture_triggered) {
		handler->gesture_triggered = 1;
		return 1;
	}

	if (handler->accum < TRIGGER_DEACTIVATION_THRESHOLD) {
		handler->gesture_triggered = 0;
	}

	return 0;
}

void send_gesture_msg(StaticGesture gesture_id) {
	uint8_t buf[4];

	GesturePacket msg;
	msg.topic = 1;
	msg.gesture_id = gesture_id;

	memcpy(&buf, &msg, sizeof(msg));
	CDC_Transmit_FS(buf, sizeof(msg));
}

uint8_t is_finger_gun() {
	// Middle finger flexed, index and thumb extended
	if (flex_thumb < 0.25 && flex_index < 0.1 && flex_middle > 0.65) return 1;


	return 0;
}

uint8_t is_thumbs_up() {
	if (flex_thumb < 0.25 && flex_index > 0.65 && flex_middle > 0.65 && grav[0] > 0.8) return 1;

	return 0;
}

uint8_t is_thumbs_middle() {
	if (flex_thumb < 0.25 && flex_index > 0.65 && flex_middle > 0.65 && grav[2] < -0.8) return 1;

	return 0;
}

uint8_t is_thumbs_down() {
	if (flex_thumb < 0.25 && flex_index > 0.65 && flex_middle > 0.65 && grav[0] < -0.8) return 1;

	return 0;
}
