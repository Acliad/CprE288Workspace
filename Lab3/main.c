/**
 * main.c
 * @author:
 * @date:
 */

#include "lcd.h"
#include "button.h"

/**
 * main.c
 * Prints the button that has been pressed
 */
int main(void) {
    volatile int button_event = 0; // Boolean to track if a button was pressed or released
    volatile int button_pressed = 0; // The highest button number that is currently pressed

    // Initialize the LCD
    lcd_init();
    // Initialize the GPIO interrupts for the buttons
    init_button_interrupts(&button_event, &button_pressed);
    // Draw the default button state on the LCD
    lcd_printf("Button #%d pressed", button_pressed);

    while(1) {
            if(button_event) {
                lcd_printf("Button #%d pressed", button_pressed);
                button_event  = 0;
            }
        }

    /** Part 1
    lcd_init();
    button_init();
    int button_pressed;
    while(1) {
        button_pressed = button_getButton();
        lcd_printf("Button #%d pressed", button_pressed);
    }

    */
    // Not necessary, but convention
    return 0;
}
