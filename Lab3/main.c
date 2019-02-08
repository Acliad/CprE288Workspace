/**
 * main.c
 * @author:
 * @date:
 */

#include "lcd.h"
#include "button.h"

volatile int button_event = 0; // Boolean to keep track of whether a hardware event has happened (button pressed)
volatile int button_num = 0; // keeps track of button pressed

/**
 * main.c
 * Prints the button that has been pressed
 */
int main(void) {
    // Hint: You may need to use pointers to return the button that has been pressed
    lcd_init();
    button_init();
    int button_pressed;

    while(1) {
        button_pressed = button_getButton();
        lcd_printf("Button #%d pressed", button_pressed);
    }

    // Not necessary, but convention
    return 0;
}
