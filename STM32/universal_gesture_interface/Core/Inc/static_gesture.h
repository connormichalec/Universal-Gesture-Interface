/*
 * static_gesture.h
 *
 *  Created on: Apr 25, 2026
 *      Author: kobes
 */

#ifndef INC_STATIC_GESTURE_H_
#define INC_STATIC_GESTURE_H_

#include "stm32f4xx_hal.h"
#include "usb_device.h"
#include "communication.h"

typedef struct {
	uint16_t accum;
	uint8_t gesture_triggered; 	// Boolean
} StaticEventHandler;

typedef enum {
	NONE = 0,
	THUMBS_UP = 1,
	THUMBS_MIDDLE = 2,
	THUMBS_DOWN = 3,
	FINGER_GUN = 4,
	BANG = 5
} StaticGesture;

StaticEventHandler NewStaticEventHandler();
uint8_t should_trigger_gesture(StaticEventHandler *handler, uint8_t gesture_active);
void send_gesture_msg(StaticGesture gesture_id);
uint8_t is_finger_gun();
uint8_t is_thumbs_up();
uint8_t is_thumbs_middle();
uint8_t is_thumbs_down();

#endif /* INC_STATIC_GESTURE_H_ */
