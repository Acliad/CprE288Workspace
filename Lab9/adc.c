/*
 * adc.c
 *
 * Set of function for setting up and reading from the ADC module
 *  Created on: Mar 8, 2019
 *      Author: jmartin3, irex
 */

#include "adc.h"

/**
 * Initialize ADC0 to use SS0 on PB4
 */
void adc_init(void){

    //Turn on the clock for GPIO Port B
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;
    //Turn on the clock for ADC0
    SYSCTL_RCGCADC_R |= BIT0;

    //Enable alternate function for GPIO PB4
    GPIO_PORTB_AFSEL_R |= BIT4;

    //Configure the pin PB4 for input and analog mode
    GPIO_PORTB_DIR_R &= ~BIT4;
    GPIO_PORTB_DEN_R &= ~BIT4;
    GPIO_PORTB_AMSEL_R |= BIT4;

    //Don't use PB4 as the trigger source, use the program
    GPIO_PORTB_ADCCTL_R &= ~BIT4;

    //Disable SS0, sample sequencer 0, for configuration
    ADC0_ACTSS_R &= ~BIT0;

    //Set SS0 to trigger from ADCPSSI
    ADC0_EMUX_R &= 0xFFF0;

    //Sample channel 10
    ADC0_SSMUX0_R |= 0xA;

    //Clear sample sequence control register
    ADC0_SSCTL0_R = 0x0;

    //Set sample sequencer to end and flag after first sample
    ADC0_SSCTL0_R |= 0x6;

    //Re-enable ADC0 SS0
    ADC0_ACTSS_R |= 0x1;
}

void adc_setAvg(int avg_rate) {
    if (avg_rate < 8) {
        ADC0_SAC_R = avg_rate;
    }
}

int adc_read(void){

    //Initiate SS0 conversion sequence
    ADC0_PSSI_R = 0x1;

    //Wait for SS0 ADC conversions to complete
    while(~ADC0_RIS_R & BIT0){
        //waiting
    }

    //Clear the raw interrupt status flag for SS0
    ADC0_ISC_R = BIT0;

    //Return channel 10 from FIFO buffer
    return ADC0_SSFIFO0_R;
}


