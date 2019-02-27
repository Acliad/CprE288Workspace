#include "lcd.h"
#include "uart.h"

#define BUFFER_SIZE 20

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
    char arr[BUFFER_SIZE];
    int index = 0;

    init_uart1_int(&received);

    while (1)
    {
        //TODO: Comment
        if (received)
        {
            if (received == '\r')
            {
                //TODO: Make this a function and call it in the if(index == BUFFER_SIZE) check
                lcd_printf("received: %.*s", index, arr);
                int i;
                for (i = 0; i < index; i++)
                {
                    uart_sendChar(arr[i]);
                }
                uart_sendChar('\n');
                uart_sendChar('\r');
                index = 0;
            }
            else
            {
                arr[index] = received;
                index++;
            }
            received = 0;
        }

        if (index == BUFFER_SIZE)
        {
            lcd_printf("received: %.*s", index, arr);
            int i;
            for (i = 0; i < index; i++)
            {
                uart_sendChar(arr[i]);
            }
            uart_sendChar('\n');
            uart_sendChar('\r');
            index = 0;
        }
    }

    /* Parts 1 and 2


     while(1) {

     //if we aren't at the BUFFER_SIZEth character
     if(index != BUFFER_SIZE){
     received = uart_receive();

     }

     //if the most recent character is return, or we are at the BUFFER_SIZEth character
     if(received == '\r' || index == BUFFER_SIZE){
     lcd_printf("received: %.*s", index, arr);
     int i;
     for(i = 0; i < index; i++) {
     uart_sendChar(arr[i]);
     }
     uart_sendChar('\n');
     uart_sendChar('\r');
     index = 0;
     }
     else{
     arr[index] = received;
     index++;
     }
     }
     */
    return 0;
}
