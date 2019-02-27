/*
 * uart.c
 *
 *  Created on: Feb 22, 2019
 *      Author: jmartin3
 */


#include "uart.h"

// Pointer to the char received
volatile char *received_char;

#define BIT0        0x01
#define BIT1        0x02
#define BIT2        0x04
#define BIT3        0x08
#define BIT4        0x10
#define BIT5        0x20
#define BIT6        0x40
#define BIT7        0x80

//sets up the uart ports
void uart_init(void){
    static uint8_t initialized = 0;

    //Check if already initialized
    if(initialized){
        return;
    }

    //enable clock to uart1 module
    SYSCTL_RCGCUART_R |= BIT1;

    //enable clock to port b of GPIO module
    SYSCTL_RCGCGPIO_R |= BIT1;

    //enable the alternate function for PB0 and PB1
    GPIO_PORTB_AFSEL_R |= (BIT0|BIT1);

    //configure PB0 and PB1 to be receive and transmit respectively
    GPIO_PORTB_PCTL_R |= 0x11;

    //enable PB0 and PB1 for i/o
    GPIO_PORTB_DEN_R |= BIT0 | BIT1;

    //set PB0 as input
    GPIO_PORTB_DIR_R &= ~BIT0;

    //set PB1 as output
    GPIO_PORTB_DIR_R |= BIT1;

    //disable uart before configuration
    UART1_CTL_R &= ~(BIT0);


    /*
    //integer baud rate register for uart1 (9,600) (16 MHz / (16 * 9,600))
    UART1_IBRD_R  = 104;

    //fractional baud rate register for uart1 (9,600)
    UART1_FBRD_R = 11;
    */


    //integer baud rate register for uart1 (115,200) (16 MHz / (16 * 115,200))
    UART1_IBRD_R  = 8;

    //fractional baud rate register for uart1 (115,200)
    UART1_FBRD_R  = 44;



    //configure line control
    UART1_LCRH_R = 0x60;

    //configure uart1 to use system clock
    UART1_CC_R = 0;

    //enable uart1
    UART1_CTL_R |= UART_CTL_RXE | UART_CTL_TXE | UART_CTL_UARTEN;


    initialized = 1;

}

/*
 * Sets up UART1 to use interrupts
 */
void init_uart1_int(char *char_tracker) {
    // Initialize UART1
    uart_init();

    // Get the address of the given variable for putting in the char data
    received_char = char_tracker;

    //configure the interrupt mask register to allow receive interrupts
    UART1_IM_R |= UART_ICR_RXIC;

    // Enable UART1 to interrupt
    NVIC_EN0_R |= BIT6;

    //Bind the interrupt to the handler
    IntRegister(INT_UART1, uart1_handler);
}

/*
 * sends a character over the uart port to the putty terminal
 */
void uart_sendChar(char data){

    //wait until there is room to send data
    while(UART1_FR_R & 0x20){
    }

    //send data
    UART1_DR_R = data;
}

/*
 * receives a character in the uart port
 */
char uart_receive(void){
    char received;

    while(!(UART1_FR_R & BIT6)){
    }

    received = (char) UART1_DR_R & 0xFF;

    return received;
}

/*
 * handles the uart1 interrupt
 */
void uart1_handler(){
    *received_char = (char) UART1_DR_R & 0xFF;

    UART1_ICR_R |= UART_ICR_RXIC;
}

