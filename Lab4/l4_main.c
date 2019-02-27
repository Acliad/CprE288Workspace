#include "lcd.h"
#include "uart.h"

/**
 * l4_main.c
 *
 * Lab 4 UART transmit and receive
 * @author jmartin3, irex
 * @date  Feb, 22, 2019
 */
int main(void)
{
    //Initialize the LCD. This also clears the screen
    lcd_init();

    //Initializes UART1 for transmit and receive
    uart_init();

    char received;

    while(1) {
        received = uart_receive();
        lcd_printf("received: %c", received);
    }
	
	return 0;
}
