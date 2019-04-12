/*
 * l9_main.c
 *
 * Start putting together all previous labs to scan in sensor data
 *
 *  Created on: Apr 5, 2019
 *      Author: jmartin3, irex
 */

#include "lcd.h"
#include "adc.h"
#include <inc/tm4c123gh6pm.h>
#include <math.h>
#include "timer.h"
#include "uart.h"
#include "ping.h"
#include "servo.h"

#define NUM_SAMPLES 50.0000


float map_distance(int raw);

// Used to hold a reading of IR and Sonar data
typedef struct distance
{
    double ir_distance;
    double sonar_distance;
} distance_t;

int main(void)
{
    distance_t distances[91];
    int degree = 0;
    int servo_pos = 0;
    char line[40]; // Buffer for output data
    double ir_avg_raw;

    lcd_init();
    adc_init();
//    adc_setAvg(6);
    timer_init();
    uart_init(115200); // 115200 buadrate for WiFi
    ping_init();
    servo_init();

    servo_move(servo_pos); // Set servo to begining of sweep

    while (1)
    {

        if (degree == 0)
        { // Print header at the top
            timer_waitMillis(1000);
            sprintf(line, "%-20s%-20s%-20s\r\n", "Degrees", "IR Distance (cm)",
                    "Sonar Distance(cm)");
            uart_sendStr(line);
        }

        // Get and average IR sensor data
        int i;
        unsigned int total = 0;
        for (i = 0; i < NUM_SAMPLES; i++)
        {
            total += adc_read(80);
        }
        // Average of all points in readings array
        ir_avg_raw = total / NUM_SAMPLES;
        lcd_printf("IR (cm): %d", ir_avg_raw);

        // Get sonar data
        distances[degree].sonar_distance = ping_read(1000);

        distances[degree].ir_distance = ir_avg_raw; // Get IR data

        // Format and send distance data
        sprintf(line, "%-20d%-20f%-20f\r\n", degree * 2, distances[degree].ir_distance, distances[degree].sonar_distance);
        uart_sendStr(line);

        degree = (degree + 1) % 91;
        servo_move(degree * 2);
        timer_waitMillis(20);
    }
}

// Maps the data points
float map_distance(int raw)
{
    return (1.217e8) * pow(raw, -2.143) + 5;
}



