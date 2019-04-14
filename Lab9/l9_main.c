/*
 * l9_main.c
 *
 * Start putting together all previous labs to scan in sensor data
 *
 *  Created on: Apr 5, 2019
 *      Author: jmartin3, irex
 */

#include "ir.h"
#include "lcd.h"
#include "ping.h"
#include "servo.h"
#include "timer.h"
#include "uart.h"
#include <inc/tm4c123gh6pm.h>
#include <math.h>

#define NUM_SAMPLES 10
#define MAX_NUM_OBJECTS 10
#define EDGE_THRESHOLD

typedef struct object
{
    unsigned int start_i;
    unsigned int end_i;
    unsigned int distance;
    unsigned int index;
} object_t;

// Used to hold a reading of IR and Sonar data
typedef struct distance
{
    double ir_distance;
    double sonar_distance;
} distance_t;

void proccess_data(distance_t* data, int length);

int main(void)
{
    distance_t distances[181]; // Array of all data points
    int degree = 0;           // Index of data
    int servo_pos = 0;        // Current servo position in degres
    char line[40];            // Buffer for output data
    double ir_cm;

    lcd_init();
    ir_init();
    //    ir_setAvg(6);
    timer_init();
    uart_init(115200); // 115200 buadrate for WiFi
    ping_init();
    servo_init();

    servo_move(servo_pos); // Set servo to begining of sweep

    while (degree < 181)
    {

        if (degree == 0)
        { // Print header at the top
            timer_waitMillis(1000);
            sprintf(line, "%-20s%-20s%-20s\r\n", "Degrees", "IR Distance (cm)",
                    "Sonar Distance(cm)");
            uart_sendStr(line);
        }

        // Get IR data
        ir_cm = ir_read_avgCM(85, NUM_SAMPLES);
        distances[degree].ir_distance = ir_cm; // Get IR data

        lcd_printf("IR (cm): %.2f", ir_cm);

        // Get sonar data
        distances[degree].sonar_distance = ping_read(100);

        // Format and send distance data
        sprintf(line, "%-20d%-20.2f%-20.2f\r\n", degree,
                distances[degree].ir_distance,
                distances[degree].sonar_distance);
        uart_sendStr(line);

        degree = (degree + 1); // % 91;
        servo_move(degree);
        timer_waitMillis(1);
    }
    proccess_data(distances, 181);
}

void proccess_data(distance_t data[], int length)
{
    object_t object[MAX_NUM_OBJECTS] = { 0 };
    unsigned int object_num = 0;
    unsigned int distance_sum = 0;
    uint8_t processing = 0;

    int i = 0;
    for (i = 0; i < length; i++)
    {
        if (data[i].ir_distance < 75.00)
        {
            if (!processing)
            {
                processing = 1;
                object[object_num].start_i = i;
                object[object_num].index = object_num + 1;
            }
            distance_sum += data[i].sonar_distance;
        }
        else if (processing)
        {
            processing = 0;
            object[object_num].end_i = i;
            unsigned int num_samples = i - (object[object_num].start_i);
            if (num_samples > 0)
            {
                object[object_num].distance = distance_sum / (num_samples);
                object_num++;
            }
            distance_sum = 0;
        }
    }

    i = 0;
    while (1)
    {
        int object_index = object[i].index;
        int ang_width = object[i].end_i - object[i].start_i;
        float distance = object[i].distance;
        float linear_width = distance * ang_width * M_PI / 180.00;
        lcd_printf("Object #: %d\nAngular Width: %d\nWidth: %.2f\nDistance: %.2f", object_index,
                   ang_width, linear_width, distance);

        i = (i + 1) % object_num;
        timer_waitMillis(1500);
    }
}
