/*
 * ping.c
 *
 *  Created on: Mar 15, 2019
 *      Author: jmartin3
 */

#include "ping.h"

#define BIT0        0x01
#define BIT1        0x02
#define BIT2        0x04
#define BIT3        0x08
#define BIT4        0x10
#define BIT5        0x20
#define BIT6        0x40
#define BIT7        0x80

#define PB3         BIT3

/*
 *
 */
void ping_init(void){

    static uint8_t initialized = 0;

    //Check if already initialized
    if(initialized){
        return;
    }

    //Turn on PORTB system clock
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;

    //Turn on Timer3B system clock
    SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R3;

    //Clear PB3 direction
    GPIO_PORTB_DIR_R &= ~PB3;

    //Disable PB3 digital function
    GPIO_PORTB_DEN_R &= ~PB3;

    initialized = 1;
}


/*
 *
 */
void ping_read(void){

    //Disable the alternate function for PB3
    GPIO_PORTB_AFSEL_R &= ~PB3;

    //Set PB3 direction as output
    GPIO_PORTB_DIR_R |= PB3;

    //Enable PB3 digital function
    GPIO_PORTB_DEN_R |= PB3;

    //Set PB3 to high for trigger pulse
    GPIO_PORTB_DATA_R |= PB3;

    //Set Timer3B to count down from (5 microseconds/62.5 nanoseconds) = 80
    ping_delayMicros(5);

    //Set PB3 to low for trigger pulse
    GPIO_PORTB_DATA_R &= ~PB3;

    //Disable Timer3
    TIMER3_CTL_R = 0;
}

// Busy-wait for the given number of microsecnds
void ping_delayMicros(unsigned int delay_time) {
    //Disable Timer3 for setup
    TIMER3_CTL_R = 0;

    //Configure Timer3 to 16-bit mode
    TIMER3_CFG_R = TIMER_CFG_16_BIT;

    //Configure Timer3B for one-shot, count down
    TIMER3_TBMR_R = TIMER_TBMR_TBMR_PERIOD;

    //Set Timer3B to count down from 1 us
    TIMER3_TBILR_R = delay_time;

    //Set the prescaler to 15 (period = 1us)
    TIMER3_TBPR_R = 15;

    //Enable Timer3B to start counting
    TIMER3_CTL_R |= TIMER_CTL_TBEN;

    //Wait for Timer3B to count down to an interrupt
    while(!(TIMER3_RIS_R & TIMER_RIS_TBTORIS));

    //Disable Timer3
    TIMER3_CTL_R = 0;
}

