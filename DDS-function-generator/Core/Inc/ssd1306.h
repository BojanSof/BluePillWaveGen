/*
 * ssd1306.h
 *
 *  Created on: Jan 17, 2022
 *      Author: bojan
 */

#ifndef INC_SSD1306_H_
#define INC_SSD1306_H_

#include "stm32f1xx_hal.h"				// needed for I2C
#include "ssd1306_font.h"				// for Font

#define SSD1306_I2C_ADDR 		(0x3C << 1)	// I2C address
#define SSD1306_SCREEN_WIDTH	128			// Screen width, in pixels
#define SSD1306_SCREEN_HEIGHT	64			// Screen height, in pixels
#define SSD1306_NUMBER_PAGES		(SSD1306_SCREEN_HEIGHT / 8)
#define SSD1306_SCREEN_BUFFER_SIZE	(SSD1306_SCREEN_WIDTH * SSD1306_SCREEN_HEIGHT / SSD1306_NUMBER_PAGES)	// refer to the description below

/**************************************************
 * p. 20 in datasheet, refer to step 5
 * After initiating i2c communication
 * control byte is send:
 * C0 D/C# 0 0 0 0 0 0
 * C0 - if zero, no more control bytes are send,
 * 		and it allows continuous stream of data
 * 		to be sent
 * D/C# - if zero, command is send, otherwise data
 **************************************************/
#define SSD1306_CONTROL_CMD_SINGLE					0x10
#define SSD1306_CONTROL_CMD_CONT					0x00
#define SSD1306_CONTROL_DATA_CONT					0x40

/************************************
 * starting with p. 28 in datasheet
 ************************************/
// 1. Fundamental Command Table
#define SSD1306_SET_CONTRAST						0x81
#define SSD1306_ENTIRE_DISPLAY_OFF					0xA4
#define SSD1306_ENTIRE_DISPLAY_ON					0xA5
#define SSD1306_SET_NORMAL_DISPLAY					0xA6
#define SSD1306_SET_INVERSE_DISPLAY					0xA7
#define SSD1306_DISPLAY_ON 							0xAF
#define SSD1306_DISPLAY_OFF 						0xAE

// 2. Scrolling Command Table
#define SSD1306_RIGHT_HORIZONTAL_SCROLL				0x26	// initiate setup for right horizontal scroll
#define SSD1306_LEFT_HORIZONTAL_SCROLL				0x27	// initiate setup for left horizontal scroll
#define SSD1306_VERTICAL_RIGHT_HORIZONTAL_SCROLL	0x29	// initiate setup for vertical and right horizontal scroll
#define SSD1306_VERTICAL_LEFT_HORIZONTAL_SCROLL		0x2A	// initiate setup for vertical and left horizontal scroll
#define SSD1306_DEACTIVATE_SCROLL					0x2E
#define SSD1306_ACTIVATE_SCROLL						0x2F
#define SSD1306_VERTICAL_SCROLL_AREA				0xA3	// initialize setup for vertical scroll area

// 3. Addressing Setting Command Table
#define SSD1306_SET_LOWER_COL_ADDR					0x00	// should be ORed with value in range 0x0 - 0xF (refer datasheet, p. 34)
#define SSD1306_SET_HIGHER_COL_ADDR					0x10	// should be ORed with value in range 0x0 - 0xF (refer datasheet, p. 34)
#define SSD1306_SET_MEM_ADDR_MODE					0x20	// initiate memory addressing mode setup (refer p. 34-35 in datasheet)
#define SSD1306_HORIZONTAL_ADDR_MODE				0x00
#define SSD1306_VERTICAL_ADDR_MODE					0x01
#define SSD1306_PAGE_ADDR_MODE						0x10
#define SSD1306_SET_COL_ADDR						0x21	// initiate column start and end address setup
#define SSD1306_SET_PAGE_ADDR						0x22	// initiate page start and end address setup
#define SSD1306_SET_PAGE_START_ADDR					0xB0	// should be ORed with value in range 0x0 - 0x7 (refer datasheet, p. 36)

// 4. Hardware Configuration (Panel resolution & layout related) Command Table
#define SSD1306_SET_DISPLAY_START_LINE				0x40
#define SSD1306_SET_SEGMENT_NOREMAP					0xA0
#define SSD1306_SET_SEGMENT_REMAP					0xA1
#define SSD1306_SET_MUX_RATIO						0xA8	// initiate mux ratio setup
#define SSD1306_SET_COM_NOREMAP						0xC0
#define SSD1306_SET_COM_REMAP						0xC8
#define SSD1306_SET_DISPLAY_OFFSET					0xD3	// initiate display offset setup
#define SSD1306_SET_COM_PINS_HW_CONFIG				0xDA	// initiate com pin configuration setup

// 5. Timing & Driving Scheme Setting Command Table
#define SSD1306_SET_DISPLAY_CLOCK_DIVIDE			0xD5	// initiate clock divide ratio setup
#define SSD1306_SET_PRECHARGE_PERIOD				0xD9	// initiate precharge period setup
#define SSD1306_SET_VCOMH_DESELECT_LEVEL			0xDB	// initiate Vcomh deselect level setup
#define SSD1306_NOP									0xE3

// Charge pump setting
#define SSD1306_CHARGE_PUMP_SETTING					0x8D
#define SSD1306_CHARGE_PUMP_ENABLED					0x14
#define SSD1306_CHARGE_PUMP_DISABLED				0x10

/**********************************************************************
 * Structure containing all the relevant variables for the OLED display
 * ********************************************************************
 * 		Screen Buffer
 *
 * 		*****************************************************
 * 		*						Page 0						*
 * 		*****************************************************
 * 		*						Page 1						*
 * 		*****************************************************
 * 		*						Page 2						*
 * 		*****************************************************
 * 		*						Page 3						*
 * 		*****************************************************
 * 		*						Page 4						*
 * 		*****************************************************
 * 		*						Page 5						*
 * 		*****************************************************
 * 		*						Page 6						*
 * 		*****************************************************
 * 		*						Page 7						*
 * 		*****************************************************
 *
 * 		Num. of pages: 		8
 * 		Num. of columns: 	128 (called segments in datasheet)
 * 		Num. of rows:		64
 *
 * 		Each page consists of 128 x 8 pixels.
 *
 * 		Each element of the screen buffer will contain the state
 * 		of the pixels (on or off) in the columns of the pages.
 *
 * 		To address specific pixel in the buffer, on coordinates (x, y),
 * 		use the following formula to calculate its index:
 *
 * 		x + (y / 8) * SSD1306_SCREEN_WIDTH
 *
 * 		and its bit position:
 *
 * 		y % 8
 *
 ***********************************************************************/
typedef struct {
	I2C_HandleTypeDef *hi2c;
	uint8_t screenBuffer[SSD1306_SCREEN_BUFFER_SIZE];
	uint8_t curX, curY;			// current coordinates on the screen (measured from top left corner)
	const Font *font;			// which font to use for text
} SSD1306;

// Interface functions
uint8_t SSD1306_Init(SSD1306* dev, I2C_HandleTypeDef *hi2c, const Font *font);
uint8_t SSD1306_UpdateScreen(SSD1306 *dev);
void 	SSD1306_SetCursor(SSD1306 *dev, uint8_t x, uint8_t y);
void	SSD1306_ClearBuffer(SSD1306 *dev);
void 	SSD1306_SetPixelState(SSD1306 *dev, uint8_t x, uint8_t y, uint8_t state);
void	SSD1306_WriteChar(SSD1306 *dev, char c);
void	SSD1306_WriteString(SSD1306 *dev, const char *str);
void	SSD1306_FillRegion(SSD1306 *dev, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t state);
void	SSD1306_InvertRegion(SSD1306 *dev, uint8_t x, uint8_t y, uint8_t width, uint8_t height);


// Low-level functions
HAL_StatusTypeDef SSD1306_WriteCommand(SSD1306 *dev, uint8_t cmd);
HAL_StatusTypeDef SSD1306_WriteData(SSD1306 *dev, uint8_t *data, uint8_t size);

#endif /* INC_SSD1306_H_ */
