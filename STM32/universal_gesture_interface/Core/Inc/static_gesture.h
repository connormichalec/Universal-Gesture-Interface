/*
 * static_gesture.h
 *
 *  Created on: Apr 25, 2026
 *      Author: kobes
 */

#ifndef INC_STATIC_GESTURE_H_
#define INC_STATIC_GESTURE_H_

#include "stm32f4xx_hal.h"

typedef struct {
	uint16_t accum;
	uint8_t gesture_triggered; 	// Boolean
} StaticEventHandler;

typedef enum {
	NONE = 0,
	THUMBS_UP = 1,
	THUMBS_DOWN = 2,
	FINGER_GUN = 3
} StaticGesture;

StaticEventHandler NewStaticEventHandler();
uint8_t should_trigger_gesture(StaticEventHandler *handler, uint8_t gesture_active);
uint8_t is_finger_gun();

#endif /* INC_STATIC_GESTURE_H_ */
