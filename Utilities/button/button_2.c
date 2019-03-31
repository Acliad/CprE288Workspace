/*
 *  button.c
 *  @author: Eric Middleton, Zhao Zhang, Chad Nelson, & Zachary Glanz.
 *  @date: Jul 18, 2016
 *
 *  @edit: Lindsey Sleeth and Sam Stifter on 02/04/2019
 */

#include "button.h"

volatile int *button_event_ptr;
volatile int *button_num_ptr;

// The buttons are on PORTE 0:5
// GPIO_PORTE_DATA_R -- the register that must be checked on port E to determine
// which buttons are being pressed

#define BIT0 0x01
#define BIT1 0x02
#define BIT2 0x04
#define BIT3 0x08
#define BIT4 0x10
#define BIT5 0x20
#define BIT6 0x40
#define BIT7 0x80
#define DEBOUNCE_TIME 5

uint8_t prev_Button; // must be set yourself in button_getButton()
uint8_t button;      // current button being pressed, must be set yourself in
                     // button_getButton()

/**
 * Initialize PORTE and configure bits 0-5 to be used as inputs for the buttons.
 */
void button_init() {
    static uint8_t initialized = 0;
    timer_init();

    // Check if already initialized
    if (initialized) {
        return;
    }

    // To initialize and configure GPIO PORTE, visit pg. 656 in the Tiva
    // datasheet. Follow steps in 10.3 for initialization and configuration.
    // Some steps have been outlined below. Ignore all other steps in
    // initialization and configuration that are not listed below. You will
    // learn more about additional steps in a later lab.

    // Turn on PORTE system clock
    SYSCTL_RCGCGPIO_R |= BIT4;

    // Set the buttons to inputs and enable
    GPIO_PORTE_DIR_R &= ~0x3F;
    // Pins used: PE0, PE1, PE2, PE3, PE4, PE5
    GPIO_PORTE_DEN_R |= 0b00111111;

    initialized = 1;
}

/**
 * Interrupt handler -- executes when a hardware event occurs (a button is
 * pressed)
 */
void init_button_interrupts(int *button_event_addr, int *button_num_addr) {
    // Store the pointer to the given button_event boolean
    button_event_ptr = button_event_addr;
    // Store the pointer to the given button_num int
    button_num_ptr = button_num_addr;

    // Set up the GPIO pins
    button_init();

    // Disable interrupts on PB0-PB5 during setup
    GPIO_PORTE_IM_R &= ~0x3F;

    // Set pins 0-5 to use edge sensing
    GPIO_PORTE_IS_R &= 0b11000000;

    // Set pins 0-5 to use both edges. We want to update the LCD
    // when a button is pressed, and when the button is released.
    GPIO_PORTE_IBE_R |= 0x3F;

    // Clear the interrupts
    GPIO_PORTE_ICR_R |= 0x3F;

    // Unmask the bits for pins 0-5
    GPIO_PORTE_IM_R |= 0b00111111;

    // Enable GPIO port E interrupt
    NVIC_EN0_R |= BIT4;

    // Bind the interrupt to the handler.
    IntRegister(INT_GPIOE, gpioe_handler);
}

/**
 * Handles a hardware interrupt that occurs from a button being pressed
 */
void gpioe_handler() {
    // Last time a button event happened for debouncing
    static uint32_t last_time_pressed = 0;
    uint32_t current_time = timer_getMillis();

    // Clear interrupt status register
    GPIO_PORTE_ICR_R = 0xFF;
    uint8_t button_states = button_getButton();

    if (current_time - timer_pressed > DEBOUNCE_TIME) {
        // Set the button_event flag
        *button_event_ptr = 1;
        // Store the value of the highest button pressed at the given button_num
        // address
        *button_num_ptr = button_states;
    }
    last_time_pressed = current_time; // Update last button press time
}

/**
 * returns a 6-bit bit field, representing the push buttons. A 1 means the
 * corresponding button is pressed.
 */
uint8_t button_checkButtons() {
    return (~GPIO_PORTE_DATA_R) & (BIT6 - 1); // Return the button status
}

/**
 * @brief Returns the position of the leftmost button being pushed. A 6 is the
 * leftmost button, 1 is the rightmost button. Return 0 if no button is being
 * pressed.
 *
 * @return the position of the leftmost button being pushed.
 */
uint8_t button_getButton() {
    //TODO: Implement debouncing
    int i;
    for (i = 5; i >= 0; i--) {
        // Check each bit from bit 5 to bit 0
        if (~GPIO_PORTE_DATA_R & (1 << i)) {
            // Return the first bit that is set to 0
            return i + 1;
        }
    }
    // No bits 0 (no buttons pressed), return 0
    return 0;
}
