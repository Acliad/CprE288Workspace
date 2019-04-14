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

int main(void) {
  lcd_init();
  timer_init();
  ping_init();


  while (1) {
   lcd_printf("Distance: %.2f", ping_read(100));
    timer_waitMillis(10);
  }
}
