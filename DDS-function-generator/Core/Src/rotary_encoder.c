/*
 * rotary_encoder.c
 *
 *  Created on: Feb 1, 2022
 *      Author: bojan
 */

#include "rotary_encoder.h"

/********************************
 * Initialize the rotary encoder
 * structure
 ********************************/
void RotaryEncoder_Init(volatile RotaryEncoder *enc, uint8_t ppr,
						TIM_HandleTypeDef *htimEncoder, TIM_HandleTypeDef *htimTimeBase,
						GPIO_TypeDef *btnPort, uint16_t btnPin) {
	enc->ppr = ppr;
	enc->htimEncoder = htimEncoder;
	enc->htimTimeBase = htimTimeBase;
	enc->btnPort = btnPort;
	enc->btnPin = btnPin;

	enc->previousTimerVal = 0;
	enc->rotationSpeed = 0;

	enc->change = 0;
	enc->btnChange = 0;
}
