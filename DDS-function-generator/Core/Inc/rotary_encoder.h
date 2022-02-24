/*
 * rotary_encoder.h
 *
 *  Created on: Feb 1, 2022
 *      Author: bojan
 */

#ifndef INC_ROTARY_ENCODER_H_
#define INC_ROTARY_ENCODER_H_

#include "stm32f1xx_hal.h"


/************************************
 * Structure for holding all
 * rotary encoder related parameters
 ************************************/
typedef struct {
	uint8_t ppr;				// pulses per revolution
	uint16_t previousTimerVal;
	int32_t rotationSpeed;		// rotation speed, in revolutions per ms
	uint8_t change;				// 1 if rotation was detected; must be cleared after using it
	uint8_t btnChange;			// 1 if the push button has been pressed

	TIM_HandleTypeDef *htimEncoder;		// timer handle configured in encoder mode
	TIM_HandleTypeDef *htimTimeBase;	// timer handle configured as time base - used for determining speed
	GPIO_TypeDef *btnPort;				// port on which the push button is connected
	uint16_t btnPin;					// pin of the port
} RotaryEncoder;

/************************
 * Interface functions
 ************************/
void RotaryEncoder_Init(volatile RotaryEncoder *enc, uint8_t ppr,
						TIM_HandleTypeDef *htimEncoder, TIM_HandleTypeDef *htimTimeBase,
						GPIO_TypeDef *btnPort, uint16_t btnPin);

#endif /* INC_ROTARY_ENCODER_H_ */
