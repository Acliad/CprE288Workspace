/**
 * l7_main.c
 *
 * Lab 7: Start using timers to read the pulse width from the PING)) sensor
 *
 * @author jmartin3, irex
 * @date  Mar, 15, 2019
 */
#include "lcd.h"
#include <inc/tm4c123gh6pm.h>
#include "ping.h"

int main(void)

{
    ping_init();

    while(1) {
        ping_read();
        ping_delayMicros(1000000);
    }
}
