/*
 * servo.c
 *
 *  Created on: Mar 29, 2019
 *      Author: jmartin3, Isaac Rex
 */

#include "servo.h"
#define PB5 0x20
#define START_VAL 320000
#define ZERO_DEG 304000

void servo_init(void) {
    static uint8_t initialized = 0;

    if (!initialized) {
        SYSCTL_RCGCGPIO_R |=
            SYSCTL_RCGCGPIO_R1; // Turn on clock for GPIO Port B
        SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R1; // Turn on clock to TIMER1
        GPIO_PORTB_DIR_R |= PB5;                   // Set PB5 to output
        GPIO_PORTB_AFSEL_R |= PB5;                 // Enable alt function on PB5
        GPIO_PORTB_PCTL_R |= GPIO_PCTL_PB5_T1CCP1; // Set PB5 to PWM mode
        GPIO_PORTB_DEN_R |= PB5;                   // Digital enable PB5
        TIMER1_CTL_R &= ~TIMER_CTL_TBEN;           // Disable TIMER1B for setup
        TIMER1_CFG_R = TIMER_CFG_16_BIT;           // Set as 16-bit timer
        TIMER1_TBMR_R = 0;          // clear the TimerB mode register
        TIMER1_TBMR_R |= 0xA;       // PWM mode, edge-count mode, periodic mode
        TIMER1_CTL_R &= ~0x4000;    // set PWM output level to non-inverted
        TIMER1_TBPR_R = (START_VAL >> 16) & 0xFF;        // Bits 17:24 of timer start value
        TIMER1_TBILR_R = START_VAL & 0xFFFF;    // Lower 16 bits of the timer start value
        TIMER1_TBPMR_R = 0x4;       // Bits 17:24 of match value
        TIMER1_TBMATCHR_R = 0xA380; // lower 16 bits of match value
        TIMER1_CTL_R |= TIMER_CTL_TBEN; // Enable TIMER1B for PWM mode
    }

    initialized = 1;
}

void servo_move(double degrees) {
    // Map degrees to the range of match values
    unsigned int new_match = ZERO_DEG - 88 * degrees;
    TIMER1_TBPMR_R = (new_match >> 16) & 0xFF;       // Bits 17:24 of match value
    TIMER1_TBMATCHR_R = new_match & 0xFFFF;    // Lower 16 bits of the timer start value
}