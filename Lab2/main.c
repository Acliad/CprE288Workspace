/*
 *  ======= main ========
 *
 *  Created on: Jan 30, 2019
 *  Author:     irex
 */
#include <stdlib.h>
#include "movement.h"
#include "lcd.h"
#include "timer.h"
/*
 * ======== main ========
 */
int main(void)
{
    lcd_init();
    lcd_printf("Initializing...\n");

    // Allocate memory and initialize the sensor variable
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);

    int distance_moved = move_forward(sensor_data, 200);
    lcd_printf("Traveled %dcm", distance_moved);

    /**
    int distance_moved;
    int degrees_turned;

    // Go to top left
    distance_moved = move_forward(sensor_data, 50);
    lcd_printf("At top left");
    degrees_turned = turn(sensor_data, -90);

    // Go to top right
    distance_moved += move_forward(sensor_data, 50);
    lcd_printf("At top right");
    degrees_turned += turn(sensor_data, -90);

    // Go to bottom right
    distance_moved += move_forward(sensor_data, 50);
    lcd_printf("At bottom right");
    degrees_turned += turn(sensor_data, -90);

    // Return to bottom left
    distance_moved += move_forward(sensor_data, 50);
    lcd_printf("At home");
    degrees_turned += turn(sensor_data, -90);


    // Report:
    lcd_printf("Turned %d degrees", degrees_turned);
    lcd_gotoLine(1);
    lcd_printf("Traveled %dcm", distance_moved);

     **/
    oi_free(sensor_data);
    return (0);
}
