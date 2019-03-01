#include "lcd.h"
#include "uart.h"
#include "lab5_scan_data.h"

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
    char received;
    //Initialize the LCD. This also clears the screen
    lcd_init();
    // Initialize UART1 using interrupts. Pass address of our char variable
    init_uart1_int(&received);

    const char *data1 = "dataset1.csv";
    const char *data2 = "dataset2.csv";
    const char *data3 = "dataset3.csv";
    // Line buffer
    char line[40];

    // Array to hold all the gathered data
    struct reading data[181];
    // Index of data
    int index;

    // prepare the header
    sprintf(line, "%-20s%-20s%-20s\r\n", "Degrees", "IR Distance (cm)", "Sonar Distance(cm)");
    // send the header
    uart_sendStr(line);
    // Grab all the data
    get_reading(data1, data);

    for (index = 0; index < sizeof(data)/sizeof(struct reading); index++)
    {
        sprintf(line, "%-20d%-20f%-20f\r\n", index, data[index].ir_distance,
                data[index].sonar_distance);
        uart_sendStr(line);
    }

    return 0;
}
