/*
 * menu.c
 *
 *  Created on: Feb 1, 2022
 *      Author: bojan
 */

#include "menu.h"
#include <stdio.h>

/**************************
 * Menu interface functions
 **************************/

/********************************
 * Initialize the menu structure
 ********************************/
void Menu_Init(Menu *menu, SSD1306 *display, const volatile RotaryEncoder *encoder, DDS *dds) {
	menu->display = display;
	menu->encoder = encoder;
	menu->dds = dds;

	menu->prevState = MENU_ENTRY;
	menu->pos = WAVETYPE;

	Menu_UpdateDisplay(menu);
}

/********************************
 * Update every instance affected
 * by menu change: the display
 * and dds parameters
 ********************************/
void Menu_Update(Menu *menu) {
	// first determine the next menu state
	MenuState nextState = Menu_NextState(menu);
	// update the dds parameters
	Menu_UpdateDDS(menu, nextState);
	// update the display
	if(menu->change) {
		Menu_UpdateDisplay(menu);
	}
	// set nextState as prevState
	menu->prevState = nextState;
}


/****************************
 * Determine the next menu
 * state
 ****************************/
MenuState Menu_NextState(Menu *menu) {
	// stay in the previous state by default
	MenuState nextState = menu->prevState;
	// record if any change was made
	menu->change = menu->encoder->change || menu->encoder->btnChange;
	// determine the next state
	switch(menu->prevState) {
		case MENU_ENTRY:
			if(menu->encoder->change) {
				menu->change = 1;
				if(menu->encoder->rotationSpeed < 0 && menu->pos > 0) {
					menu->pos--;
				}
				else if(menu->encoder->rotationSpeed > 0 && menu->pos < MAX_MENU_ENTRY) {
					menu->pos++;
				}
			} else if(menu->encoder->btnChange) {
				switch(menu->pos) {
					// waveform type change
					case 0:
						nextState = WAVETYPE_CHANGE;
					break;
					// waveform frequency change
					case 1:
						nextState = FREQUENCY_CHANGE;
					break;
					// waveform amplitude change
					case 2:
						nextState = AMPLITUDE_CHANGE;
					break;
					// waveform type change
					case 3:
						nextState = STATE_CHANGE;
					break;
					default:
					break;
				}
			}
		break;
		case WAVETYPE_CHANGE:
		case FREQUENCY_CHANGE:
		case AMPLITUDE_CHANGE:
		case STATE_CHANGE:
			if(menu->encoder->btnChange) {
				nextState = UPDATE_CHANGES;
			}
		break;
		case UPDATE_CHANGES:
			nextState = MENU_ENTRY;
		break;
		default:
		break;
	}
	return nextState;
}

/****************************
 * Update DDS parameters
 ****************************/
void Menu_UpdateDDS(Menu *menu, MenuState nextState) {
	if(menu->change) {
		// first, check if rotary encoder changed
		if(menu->encoder->change) {
			// next, check if waveform type should be changed
			if(	menu->prevState == WAVETYPE_CHANGE &&
				nextState == WAVETYPE_CHANGE) {
				switch(menu->dds->waveType) {
					case SINE:
						DDS_ChangeWaveform(menu->dds, SQUAREWAVE_LUT, SQUAREWAVE_LUT_SIZE, SQUARE);
					break;
					case SQUARE:
						DDS_ChangeWaveform(menu->dds, TRIANGLEWAVE_LUT, TRIANGLEWAVE_LUT_SIZE, TRIANGLE);
					break;
					case TRIANGLE:
						DDS_ChangeWaveform(menu->dds, SAWTOOTHWAVE_LUT, SAWTOOTHWAVE_LUT_SIZE, SAWTOOTH);
					break;
					case SAWTOOTH:
						DDS_ChangeWaveform(menu->dds, SINEWAVE_LUT, SINEWAVE_LUT_SIZE, SINE);
					break;
					default:
					break;
				}
			}
			// check if frequency should be changed
			else if(menu->prevState == FREQUENCY_CHANGE &&
					nextState == FREQUENCY_CHANGE) {
				uint32_t absSpeed = (menu->encoder->rotationSpeed > 0) ? menu->encoder->rotationSpeed : -menu->encoder->rotationSpeed;
				int8_t sign = (menu->encoder->rotationSpeed > 0) ? 1 : -1;
				if(absSpeed <= 10000) {
					DDS_ChangeFrequency(menu->dds, menu->dds->freq + sign*1);
				} else {
					DDS_ChangeFrequency(menu->dds, menu->dds->freq + sign*100);
				}
			}
			// check if amplitude should be changed
			else if(menu->prevState == AMPLITUDE_CHANGE &&
							nextState == AMPLITUDE_CHANGE) {
				uint32_t absSpeed = (menu->encoder->rotationSpeed > 0) ? menu->encoder->rotationSpeed : -menu->encoder->rotationSpeed;
				int8_t sign = (menu->encoder->rotationSpeed > 0) ? 1 : -1;
				if(absSpeed <= 8000) {
					DDS_ChangeAmplitude(menu->dds, menu->dds->amp + sign*10);
				} else {
					DDS_ChangeAmplitude(menu->dds, menu->dds->amp + sign*100);
				}
			}
			// check if dds state should be changed
			else if(menu->prevState == STATE_CHANGE &&
							nextState == STATE_CHANGE) {
				if(menu->dds->running) {
					DDS_Stop(menu->dds);
				} else {
					DDS_Start(menu->dds);
				}
			}
		}
		// if UPDATE_CHANGES, update the dds
		else if(nextState == UPDATE_CHANGES) {
			DDS_Update(menu->dds);
		}
	}
}

/**********************
 * Update the display
 **********************/
void Menu_UpdateDisplay(Menu *menu) {
	char buf[25];
	// clear the display
	SSD1306_ClearBuffer(menu->display);

	SSD1306_SetCursor(menu->display, 0, 0);
	SSD1306_WriteString(menu->display, "DDS WaveGen v1.0");
	SSD1306_FillRegion(menu->display, 0, 8, SSD1306_SCREEN_WIDTH, 8, 1);
	SSD1306_SetCursor(menu->display, 0, 16);
	SSD1306_WriteString(menu->display, "Waveform: "); 	// on x=60 write waveform type
	SSD1306_SetCursor(menu->display, 0, 24);
	SSD1306_WriteString(menu->display, "Frequency: ");	// on x=66 write waveform frequency
	SSD1306_SetCursor(menu->display, 0, 32);
	SSD1306_WriteString(menu->display, "Amplitude: "); 	// on x=66 write waveform amplitude
	SSD1306_SetCursor(menu->display, 0, 50);
	SSD1306_WriteString(menu->display, "State: "); 		// on x=42 write current state (idle/running)

	switch(menu->dds->waveType) {
		case SINE:
			sprintf(buf, "Sine     ");
		break;
		case SQUARE:
			sprintf(buf, "Square   ");
		break;
		case TRIANGLE:
			sprintf(buf, "Triangle ");
		break;
		case SAWTOOTH:
			sprintf(buf, "Sawtooth ");
		break;
		default:
			sprintf(buf, "Arbitrary");
		break;
	}
	SSD1306_SetCursor(menu->display, 60, 16);
	SSD1306_WriteString(menu->display, buf);
	SSD1306_SetCursor(menu->display, 66, 24);
	sprintf(buf, "%6u Hz", (unsigned int)menu->dds->freq);
	SSD1306_WriteString(menu->display, buf);
	SSD1306_SetCursor(menu->display, 66, 32);
	sprintf(buf, "%4u mV", (unsigned int)menu->dds->amp);
	SSD1306_WriteString(menu->display, buf);
	if(menu->dds->running) {
		sprintf(buf, "Running");
	} else {
		sprintf(buf, "Idle");
	}
	SSD1306_SetCursor(menu->display, 42, 50);
	SSD1306_WriteString(menu->display, buf);

	// highlight the selection,
	if(menu->pos == MAX_MENU_ENTRY) {
		SSD1306_InvertRegion(menu->display, 0, 50, SSD1306_SCREEN_WIDTH, 8);
	} else {
		SSD1306_InvertRegion(menu->display, 0, 16 + 8*menu->pos, SSD1306_SCREEN_WIDTH, 8);
	}
	SSD1306_UpdateScreen(menu->display);
}
