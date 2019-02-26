#include "lcd.h"

/**
 * main.c
 *
 * Simple 'Hello, world program'.
 * This program prints "Hello, world" to the LCD screen
 * @author 
 * @date 
 */
int main(void)
{
    //Initialize the LCD. This also clears the screen
    lcd_init();
    const char MSG[] = "Microcontrollers are lots of fun!";
	
	// Clear the LCD screen and print "Hello, world" on the LCD
	//lcd_printf("Hello, world");

    while(1) {
        lcd_rotatingBanner(MSG);
    }
	
	return 0;
}
