/*
 * uart.h
 * Header file for a UART interface.
 *
 *  @author: jmartin3, irex
 *  @date:  Feb., 22, 2019
 */

#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#include <stdbool.h>
#include "timer.h"
#include "lcd.h"
#include <inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"

//sets up the uart ports
void uart_init(void);

//sends a character over the uart port
void uart_sendChar(char data);

//receives a character in the uart port
char uart_receive(void);

//handles uart1 interrupt
void uart1_handler();

//initialize uart1 to handle interrupts
void init_uart1_int(char *char_tracker);

#endif
