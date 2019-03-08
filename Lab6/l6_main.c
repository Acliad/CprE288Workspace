/**
 * l6_main.c
 *
 * Lab 6: Lab to start learning about reading and analyzing data from an ADC source
 *
 * @author jmartin3, irex
 * @date  Mar, 8, 2019
 */
#include "lcd.h"
#include "adc.h"
#include <inc/tm4c123gh6pm.h>
#include "timer.h"

#define NUM_SAMPLES 25
#define UPDATE_DELAY 15


int main(void)
{
    int ir_readings[NUM_SAMPLES] = {0};
    int readings_sum = 0;
    int index = 0;

    //initialize the lcd screen and adc0
    lcd_init();
    adc_init();
    timer_startTimer(0);

    while(1){
        readings_sum -= ir_readings[index];
        ir_readings[index] = adc_read();
        readings_sum += ir_readings[index];
        index = (index + 1) % NUM_SAMPLES;

        if(timer_getClock() < UPDATE_DELAY) {
            lcd_printf("Raw reading of the IR sensor: %d", readings_sum/NUM_SAMPLES);
            timer_startTimer(0);
        }

    }

    return 0;
}
