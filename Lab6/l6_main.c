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
#include <math.h>
#include "timer.h"
#include "uart.h"

#define NUM_SAMPLES 50
#define UPDATE_DELAY 10

float map_distance(int raw);


int main(void)

{
    int ir_readings[NUM_SAMPLES] = {0};
    int readings_sum = 0;
    int averaged_raw = 0;
    int index = 0;
    char out_buffer[50];

    float distance = 0;
//    unsigned long last_update_time = 0;

    //initialize the lcd screen and adc0
    lcd_init();
    adc_init();
    timer_startClock();
    uart_init(115200);

    while(1){
        // Subtracts the oldest data point from the total sum
        readings_sum -= ir_readings[index];
        // Gets the new data point and store in the array
        ir_readings[index] = adc_read();
        // Adds the new data point to the total sum
        readings_sum += ir_readings[index];
        // Increments the index, loops back to zero
        index = (index + 1) % NUM_SAMPLES;

        // Average of all points in readings array
        averaged_raw = readings_sum / NUM_SAMPLES;


        // Delay for LCD
        timer_waitMillis(UPDATE_DELAY);

//        if((current_time - last_update_time) < UPDATE_DELAY) {
//            lcd_printf("Raw reading of the IR sensor: %d", averaged_raw);
//            last_update_time = current_time;
//        }

        /****** Part 2 ******/
        distance = map_distance(averaged_raw);
        lcd_printf("Raw reading: %d\nDistance: %.1f", averaged_raw, distance);

        /****** Part 3 ******/
        sprintf(out_buffer, "%d, %0.2f\n\r", averaged_raw, distance);
        uart_sendStr(out_buffer);

    }

    return 0;
}

// Maps the data points
float map_distance(int raw) {
    return (1.217e8)*pow(raw, -2.143) + 5;
}
