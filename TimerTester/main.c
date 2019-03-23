/**
 * main.c
 *
 * Lab 7: Start using timers to read the pulse width from the PING)) sensor
 *
 * @author jmartin3, irex
 * @date  Mar, 15, 2019
 */
#include "timer.h"
#include <inc/tm4c123gh6pm.h>

int main(void) {
    // Enable clock to port A
    SYSCTL_RCGCGPIO_R |= 0x01;

    // Set PA2 as output
    GPIO_PORTA_DIR_R |= 0x04;

    // Enable PA2 to be a digital pin
    GPIO_PORTA_DEN_R |= 0x04;

    // Start the clock
    timer_startCounter();
    //    lcd_init();

    unsigned int last_update = 0;
    unsigned int current_time = 0;
    while (1) {

       GPIO_PORTA_DATA_R ^= 0x04;
       timer_waitMillis(100);

        /*** Testing timer_getMicros() ***/
        //  current_time = timer_getMicros();
        //  if (current_time - last_update >= 15000) {
        //      last_update = timer_getMicros();
        //      GPIO_PORTA_DATA_R ^= 0x04;
        //  }
     }
}
