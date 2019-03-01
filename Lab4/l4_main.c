#include "lcd.h"
#include "uart.h"

#define BUFFER_SIZE 20

//
void printChars(int index, char *arr);

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
    // Holds the current received char
    char received;
    // Buffer for holder all the received chars
    char arr[BUFFER_SIZE];
    int index = 0;


    // Initialize UART1 using interrupts. Pass address of our char variable
    init_uart1_int(&received);


    while (1)
    {
        if (received) // True when an interrupt triggered and set received to a value
        {
            if (received == '\r') // If we received a carriage return
            {
                // Print out what's currently in the buffer
                printChars(index, arr);
                // Reset the buffer index
                index = 0;
            }
            else
            {
                // Store the received character in the buffer
                arr[index] = received;
                index++;
            }
            // Reset received to null so we know when a new character arrives
            received = 0;
        }

        // If we reached 20 characters
        if (index == BUFFER_SIZE)
        {
            // Print out the buffer
            printChars(index, arr);
            index = 0;
        }
    }


    //Parts 1 and 2

    /*
    while (1)
    {

        //if we aren't at the BUFFER_SIZEth character
        if (index < BUFFER_SIZE)
        {
            received = uart_receive();
        }

        //if the most recent character is return, or we are at the BUFFER_SIZEth character
        if (received == '\r' || index == BUFFER_SIZE)
        {
            printChars(index, arr);
            index = 0;
        }
        else
        {
            arr[index] = received;
            index++;
        }
    }
    */

    return 0;
}

// Iterates through the buffer printing each char, also echoes back to the putty terminal and adds a new line
void printChars(int index, char *arr){
    // Prints the given char buffer from 0 to index
    lcd_printf("received: %.*s", index, arr);

    // Send each char in the buffer one at a time
    int i;
    for (i = 0; i < index; i++)
    {
        uart_sendChar(arr[i]);
    }

    // Send a new line and carriage return for readability
    uart_sendChar('\n');
    uart_sendChar('\r');
}
