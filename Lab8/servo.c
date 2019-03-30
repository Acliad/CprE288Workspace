/*
 * servo.c
 *
 *  Created on: Mar 29, 2019
 *      Author: jmartin3
 */

#include "servo.h"
#define PB5 0x20

void servo_init(void){
    static uint8_t initialized = 0;


    if(!initialized){
        SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;   // Turn on clock for GPIO Port B
        SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R1; // Turn on clock to TIMER1
        GPIO_PORTB_DIR_R |= PB5;                  // Set PB5 to output
        GPIO_PORTB_AFSEL_R |= PB5;                // Enable alternate function for PB5
        GPIO_PORTB_PCTL_R |= 0x400000;             // Set PB5 to use alternate function 4 (PWM)
        TIMER1_CTL_R &= ~TIMER_CTL_TBEN;           // Disable TIMER1 for setup
        TIMER1_CFG_R = TIMER_CFG_16_BIT;           // Set as 16-bit timer
        TIMER1_TBMR_R = 0;       // clear the TimerB mode register
        TIMER1_TBMR_R |= 0xA;    // PWM mode, edge-count mode, periodic mode
        TIMER1_CTL_R &= 0x4000;  //set PWM output level to non-inverted
        TIMER1_TBPR_R = 0x4;     //linear extension of the timer register
        TIMER1_TBILR_R = 0xE200;  //16 bits of the timer start value
        TIMER1_TBPMR_R = 0x4;    //linear extension of the match register
        TIMER1_TBMATCHR_R = 0xA380; //16 bits of the match register
        TIMER1_CTL_R |= TIMER_CTL_TBEN;  //Enable TIMER1 for PWM mode
    }

    initialized = 1;
}

void servo_move(double degrees){

}



