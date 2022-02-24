/*
 * menu.h
 *
 *  Created on: Feb 1, 2022
 *      Author: bojan
 */

#ifndef INC_MENU_H_
#define INC_MENU_H_

#include <stdint.h>
#include "dds.h"
#include "ssd1306.h"
#include "rotary_encoder.h"

#define MAX_MENU_ENTRY 3

/********************************
 * Enumeration with all possible
 * menu states
 ********************************/
typedef enum {
	MENU_ENTRY,			// when scrolling menu entries
	WAVETYPE_CHANGE,	// when changing waveform type
	FREQUENCY_CHANGE,	// when changing waveform frequency
	AMPLITUDE_CHANGE,	// when changing waveform amplitude
	STATE_CHANGE,		// when turning dds on/off
	UPDATE_CHANGES		// after exiting any change
} MenuState;

/********************************
 * Enumeration with all possible
 * menu entries
 ********************************/
typedef enum {
	WAVETYPE,
	FREQUENCY,
	AMPLITUDE,
	STATE
} MenuEntry;

/************************************
 * Structure holding variables
 * required for updating the menu
 ************************************/
typedef struct {
	MenuState prevState;			// the previous state of the menu
	MenuEntry pos;					// which entry is selected

	SSD1306 *display;				// pointer to the display instance
	const volatile RotaryEncoder *encoder;	// pointer to the encoder instance
	DDS *dds;						// pointer to the dds instance

	uint8_t change;					// if set, some change has been made, so update
									// the display and/or dds
} Menu;

/**************************
 * Menu interface functions
 **************************/
void Menu_Init(Menu *menu, SSD1306 *display, const volatile RotaryEncoder *encoder, DDS *dds);
void Menu_Update(Menu *menu);


/**************************
 * Functions used by menu
 * update function
 *************************/
MenuState 	Menu_NextState(Menu *menu);
void		Menu_UpdateDDS(Menu *menu, MenuState nextState);
void		Menu_UpdateDisplay(Menu *menu);

#endif /* INC_MENU_H_ */
