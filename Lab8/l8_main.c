/**
 * l8_main.c
 *
 * Lab 8:
 *
 * @author jmartin3, irex
 * @date  Mar, 29, 2019
 */
#include "button.h"
#include "lcd.h"
#include "servo.h"
#include "timer.h"
#include <inc/tm4c123gh6pm.h>

#define MAX_DEG 180.00
#define MIN_DEG 0.00

int main(void) {
    volatile int button_event = 0;   // Track if button has been pressed
    volatile int button_pressed = 0; // Highest butotn pressed
    int direction = 1; // Move clockwise (-1) or counterclockwise (1)
    float degrees = MAX_DEG / 2.00; // Start at 90 degrees
    int register_val = 0;       // Value in the timer match register (24 bits)
    char str_buff[100];         // Buffer to hold the LCD output
    char *rotation_dir_str = "CCW"; // String for direction indication on LCD

    init_button_interrupts(&button_event, &button_pressed);
    servo_init();
    lcd_init();
    timer_init();

    while (1) {
        rotation_dir_str = (direction > 0 ? "CCW" : "CW");
        register_val = servo_move(degrees); // Update the servo

        sprintf(str_buff,
                "Direction: %s\nMatch Value:%d\nDegrees: %.2f\nButton: %d",
                rotation_dir_str, register_val, degrees, button_pressed);
        lcd_printf(str_buff); // Update the LCD

        if (button_event) {
            switch (button_pressed) {
            case 1: // Button 1
                degrees += 1.0 * direction;
                break;
            case 2: // Button 2
                degrees += 2.5 * direction;
                break;
            case 3: // Button 3
                degrees += 5.0 * direction;
                break;
            case 4: // Button 4
                direction *= -1;
                break;
            case 5: // Button 5
                degrees = MIN_DEG;
                break;
            case 6: // Button 6
                degrees = MAX_DEG;
                break;
            default: // Button 0
                break;
            }
            button_event = 0;
        }
    }
}
