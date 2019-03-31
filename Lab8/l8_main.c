/**
 * l8_main.c
 *
 * Lab 8:
 *
 * @author jmartin3, irex
 * @date  Mar, 29, 2019
 */
#include "lcd.h"
#include "servo.h"
#include "timer.h"
#include <inc/tm4c123gh6pm.h>


int main(void)
{
    servo_init();
    lcd_init();
    timer_init();

    int num = 180;

    while(1){
        lcd_printf("test %d", num);
        servo_move(num++);
        timer_waitMillis(20);
        num %= 200;
    }

}
