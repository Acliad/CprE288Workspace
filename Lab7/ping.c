/*
 * ping.c
 *
 *  Created on: Mar 15, 2019
 *      Author: jmartin3
 */

#include "ping.h"

#define BIT0 0x01
#define BIT1 0x02
#define BIT2 0x04
#define BIT3 0x08
#define BIT4 0x10
#define BIT5 0x20
#define BIT6 0x40
#define BIT7 0x80
#define PB3 BIT3

uint16_t *ptr_pulsewidth;
uint8_t rising_edge; // Track if the last interrupt was a rising or falling edge

// TODO: Add brief
void ping_init(volatile unsigned int *ptr) {
    static uint8_t initialized = 0;
    ptr_pulsewidth = ptr;

    // Check if already initialized
    if (initialized) {
        return;
    }

    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;   // Turn on PORTB system clock
    SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R3; // Turn on clock to TIMER3

    /* Set up GPIO PB3 */
    GPIO_PORTB_DIR_R &= ~PB3; // Clear PB3 direction
    GPIO_PORTB_DEN_R |= PB3;  // Enable PB3 digital function

    /* Set up TIMER3 for edge detect mode */
    TIMER3_CTL_R &= ~TIMER_CTL_TBEN;        // Disable while we set up
    TIMER3_CFG_R = TIMER_CFG_16_BIT;        // Configure for 16 bit mode
    TIMER3_TBMR_R = 0x00;                   // Reset TIMER3B mode
    TIMER3_TBMR_R |= TIMER_TBMR_TBMR_CAP;   // Set to input capture mode
    TIMER3_TBMR_R |= TIMER_TBMR_TBCMR;      // Set to edge-time mode
    TIMER3_TBMR_R |= TIMER_TBMR_TBCDIR;     // Set to count up mode
    TIMER3_CTL_R |= TIMER_CTL_TBEVENT_BOTH; // Set to capture on both edges
    TIMER3_TBPR_R = 0xFF;                   // Use full prescaler for 24 bits
    TIMER3_TBILR_R = 0xFFFF;                // Use the full counter range
    TIMER3_IMR_R |= TIMER_IMR_CBEIM;        // Allow interrupts from edge events
    TIMER3_ICR_R |= TIMER_ICR_CBECINT;      // Ensure interrupt flag is clear
    TIMER3_CTL_R |= TIMER_CTL_TBEN;         // Enable TIMER3

    NVIC_EN1_R |= (0x10); // Enable interrupts from TIMER3B
    IntRegister(INT_TIMER3B, ping_captureHandler); // Bind the ISR

    initialized = 1;
}

// TODO: add brief
void ping_read(void) {

    // Disable the alternate function for PB3
    GPIO_PORTB_AFSEL_R &= ~PB3;

    // Set PB3 direction as output
    GPIO_PORTB_DIR_R |= PB3;

    // Disable interrupts from TIMER3B while we pulse the pin
    TIMER3_IMR_R &= ~TIMER_IMR_CBEIM;

    // Set PB3 to high for trigger pulse
    GPIO_PORTB_DATA_R |= PB3;

    // Wait to trigger reading
    timer_waitMicros(5);

    // Set PB3 to low for trigger pulse
    GPIO_PORTB_DATA_R &= ~PB3;

    // Set PB3 direction as input
    GPIO_PORTB_DIR_R &= ~PB3;

    // Enable alternate function on PB3
    GPIO_PORTB_AFSEL_R |= PB3;

    // Enable CCP mode on PB3
    GPIO_PORTB_PCTL_R |= 0x7000;

    // Let the ISR know we're expecting a rising edge
    rising_edge = 1;

    // Clear interrupt flag
    TIMER3_ICR_R |= TIMER_ICR_CBECINT;

    // Reenable interrupts from TIMER3B
    TIMER3_IMR_R |= TIMER_IMR_CBEIM;
}

// TODO: Add brief
void ping_captureHandler(void) {
    static unsigned int capture_time = 0;
    // If an edge event triggered this interrupt
    if (TIMER3_MIS_R & TIMER_MIS_CBEMIS) {
        TIMER3_ICR_R |= TIMER_ICR_CBECINT; // Clear the interrupt flag
        if (rising_edge) {
            capture_time = TIMER3_TBR_R;
            rising_edge = 0; // Next trigger will be a falling edge
        } else {
            *ptr_pulsewidth = TIMER3_TBR_R - capture_time;
            rising_edge = 1; // Next trigger will be a rising edge
        }
        //TODO: Remove
        if (TIMER3_RIS_R & TIMER_RIS_TBMRIS) {
            TIMER3_ICR_R |= TIMER_ICR_TBMCINT;
            num_overflows++;
        }
        
    }
}
