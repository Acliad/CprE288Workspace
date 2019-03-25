/**
 * l7_main.c
 *
 * Lab 7: Start using timers to read the pulse width from the PING)) sensor
 *
 * @author jmartin3, irex
 * @date  Mar, 15, 2019
 */
#include "lcd.h"
#include "ping.h"
#include "timer.h"
#include <inc/tm4c123gh6pm.h>

// Convert the number of clock cycles (62.5ns per cycle) to ms
#define CLK_CYCL_TO_MS(a) (a / 2 * 62.5 * 1e-6)
// Convert the number of clock cycles to cm based on speed of sound
#define CLK_CYCL_TO_CM(a) (a / 2 * 62.5e-7 * 340)

volatile unsigned int num_overflows;

int main(void)
{
    volatile unsigned int pulsewidth = 0;
    char msg[100];
    timer_init();
    lcd_init();
    ping_init(&pulsewidth);

    while (1) {
        ping_read();
        timer_waitMillis(300);
        if (pulsewidth > 0) {
            sprintf(msg, "Clock cycles: %d\nmilliseconds: %0.2f\nDistance: %0.2fcm\nOverflows: %d",
                    pulsewidth, CLK_CYCL_TO_MS(pulsewidth),
                    CLK_CYCL_TO_CM(pulsewidth), num_overflows);
            lcd_printf(msg);
            pulsewidth = 0;
        }
    }
}
