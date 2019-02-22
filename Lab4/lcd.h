/*
 * lcd.h
 *
 *  @author: nbergman
 *  @date:  Mar 1, 2016
 */

#ifndef LCD_H_
#define LCD_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <inc/tm4c123gh6pm.h>
#include "timer.h"

/// Initialize PORTB0:6 to Communicate with LCD
void lcd_init(void);

///Send Char to LCD
void lcd_putc(char data);

///Send Character array to LCD
void lcd_puts(char data[]);

///Clear LCD Screen
void inline lcd_clear(void);

///Return Cursor to 0,0
void inline lcd_home(void);

///Goto Line on LCD - 0 Indexed
void lcd_gotoLine(uint8_t lineNum);

///Set cursor position - top left is 0,0
void lcd_setCursorPos(uint8_t x, uint8_t y);

//Print a formatted string to the LCD screen
void lcd_printf(const char *format, ...);

//Prints a rotating banner of text to the LCD screen
void lcd_rotatingBanner(char dispText[]);

#endif /* LCD_H_ */
