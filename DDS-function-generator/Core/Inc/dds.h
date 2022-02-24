/*
 * dds.h
 *
 *  Created on: Jan 28, 2022
 *      Author: bojan
 */

#ifndef INC_DDS_H_
#define INC_DDS_H_

#include "stm32f1xx_hal.h"		// for DMA and TIM handles

#define DDS_MAX_LUT_SIZE 	256					// maximum size for the LUT table
#define DDS_MAX_FREQ		(uint32_t)100000	// maximum waveform frequency, in Hz
#define DDS_MIN_AMP			(uint16_t)500		// minimum waveform amplitude, in mV
#define DDS_MAX_AMP			(uint16_t)3000		// maximum waveform amplitude, in mV
#define DDS_TIM_FREQ		14400000			// TIM increment frequency
#define DDS_TIM_PWM_MAX_DC	800					// TIM PWM max duty cycle value (800-1 for 100% == 3300 mV)
/********************
 * Waveform types
 ********************/
typedef enum {
	SINE,
	SQUARE,
	TRIANGLE,
	SAWTOOTH,
	ARBITRARY
} WaveformType;

/**********************************************
 * Structure for holding waveform informations
 **********************************************/
typedef struct {
	uint32_t freq;							// waveform frequency, in Hz
	uint32_t amp;							// waveform amplitude, in mV
	WaveformType waveType;					// type of waveform


	uint8_t wavetable[DDS_MAX_LUT_SIZE];	// LUT for the waveform (will be adjusted based on frequency)
	uint16_t wavetableSize;					// LUT size

	const uint8_t *unmodifiedWavetable;		// Unmodified LUT
	uint16_t unmodifiedWavetableSize;		// Unmodified LUT size

	GPIO_TypeDef *gpio;						// GPIO port which has the lower 8 pins connected to R-2R ladder
	TIM_HandleTypeDef *htim;				// handle for timer used for making DMA requests
	TIM_HandleTypeDef *htimOffset;			// handle for timer used for making pwm with average value half the amplitude
											// of the waveform

	uint8_t running;						// if 0, dds generation is not running
} DDS;

/**************************************
 * Interface functions
 **************************************/
void DDS_Init(DDS *dds, const uint8_t *wavetable, uint16_t wavetableSize, WaveformType waveType, uint32_t freq, uint32_t amp, GPIO_TypeDef *gpio, TIM_HandleTypeDef *htim, TIM_HandleTypeDef *htimOffset);
void DDS_Start(DDS *dds);
void DDS_Stop(DDS *dds);
void DDS_Update(DDS *dds);
void DDS_ChangeFrequency(DDS *dds, uint32_t freq);
void DDS_ChangeAmplitude(DDS *dds, uint32_t amp);
void DDS_ChangeWaveform(DDS *dds, const uint8_t *wavetable, uint16_t wavetableSize, WaveformType waveType);

/**************************************
 * Look-up tables (LUT)
 **************************************/
#define SINEWAVE_LUT_SIZE		256
#define SQUAREWAVE_LUT_SIZE		256
#define SAWTOOTHWAVE_LUT_SIZE	256
#define TRIANGLEWAVE_LUT_SIZE	256

extern const uint8_t SINEWAVE_LUT[SINEWAVE_LUT_SIZE];
extern const uint8_t SQUAREWAVE_LUT[SQUAREWAVE_LUT_SIZE];
extern const uint8_t SAWTOOTHWAVE_LUT[SAWTOOTHWAVE_LUT_SIZE];
extern const uint8_t TRIANGLEWAVE_LUT[TRIANGLEWAVE_LUT_SIZE];

#endif /* INC_DDS_H_ */
