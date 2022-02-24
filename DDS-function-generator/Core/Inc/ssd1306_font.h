/*
 * ssd1306_font.h
 *
 *  Created on: Jan 28, 2022
 *      Author: bojan
 */

#ifndef INC_SSD1306_FONT_H_
#define INC_SSD1306_FONT_H_

#include <stdint.h>

/*****************************************
 * Structure for holding font informations
 *****************************************/
typedef struct {
	uint8_t width;				// character width, in pixels
	uint8_t height;				// character height, in pixels
	const uint8_t *fontset;		// the actual font data
} Font;

/*****************************************
 * Pre-defined fonts
 *****************************************/
extern const Font SSD1306_FONT6x8;

// Interface Functions
void Font_Init(Font *font, uint8_t width, uint8_t height, const uint8_t *fontset);

#endif /* INC_SSD1306_FONT_H_ */
