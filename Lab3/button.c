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
// GPIO_PORTE_DATA_R -- the register that must be checked on port E to determine which buttons are being pressed

#define BIT0		0x01
#define BIT1		0x02
#define BIT2		0x04
#define BIT3		0x08
#define BIT4		0x10
#define BIT5		0x20
#define BIT6		0x40
#define BIT7		0x80

uint8_t prev_Button; //must be set yourself in button_getButton()
uint8_t button; //current button being pressed, must be set yourself in button_getButton()

/**
 * Initialize PORTE and configure bits 0-5 to be used as inputs for the buttons.
 */
void button_init() {
	static uint8_t initialized = 0;

	//Check if already initialized
	if(initialized){
		return;
	}

	// To initialize and configure GPIO PORTE, visit pg. 656 in the Tiva datasheet.
	// Follow steps in 10.3 for initialization and configuration. Some steps have been outlined below.
	// Ignore all other steps in initialization and configuration that are not listed below. You will learn more
	// about additional steps in a later lab.

	//Turn on PORTE system clock
	SYSCTL_RCGCGPIO_R |= BIT4;

	//Set the buttons to inputs and enable
	GPIO_PORTE_DIR_R &= 0x00;
	// Pins used: PE0, PE1, PE2, PE3, PE4, PE5
	GPIO_PORTE_DEN_R |= 0b00111111;

	initialized = 1;
}

/**
 * Interrupt handler -- executes when a hardware event occurs (a button is pressed)
 */
void init_button_interrupts(int *button_event_addr, int *button_num_addr) {

    #warning: "Unimplemented function: void init_button_interrupts(int *button_event_addr, int *button_num_addr) -- You must configure GPIO to detect interrupts" // delete warning after implementing
    // In order to configure GPIO ports to detect interrupts, you will need to visit pg. 656 in the Tiva datasheet.
    // Notice that you already followed some steps in 10.3 for initialization and configuration of the GPIO ports in the function button_init().
    // Additional steps for setting up the GPIO port to detect interrupts have been outlined below.
    // TODO: Complete code below

    // Mask the bits for pins 0-5
    //1. GPIO_PORTE_IM_R &=

    // Set pins 0-5 to use edge sensing
    //2. GPIO_PORTE_IS_R &=

    // Set pins 0-5 to use both edges. We want to update the LCD
    // when a button is pressed, and when the button is released.
    //3. GPIO_PORTE_IBE_R |=

    // Clear the interrupts
    //4. GPIO_PORTE_ICR_R =

    // Unmask the bits for pins 0-5
    //5. GPIO_PORTE_IM_R |=

    #warning: "Unimplemented function: void init_button_interrupts(int *button_event_addr, int *button_num_addr) -- You must configure interrupts" // delete warning after implementing
    // TODO: Complete code below
    // Enable GPIO port E interrupt
    //6. NVIC_EN0_R |=

    // Bind the interrupt to the handler.
    IntRegister(INT_GPIOE, gpioe_handler);
}

/**
 * Handles a hardware interrupt that occurs from a button being pressed
 */
void gpioe_handler() {

#warning: "Unimplemented function: void gpioe_handler() -- You must configure interrupts" // delete warning after implementing
    // Clear interrupt status register
    // GPIO_PORTE_ICR_R =
    // update *button_event_ptr = 1;
    *button_num_ptr = button_getButton();
}

/**
 * returns a 6-bit bit field, representing the push buttons. A 1 means the corresponding button is pressed.
 */
uint8_t button_checkButtons()
{
    return (~GPIO_PORTE_DATA_R) & (BIT6 - 1); //Return the button status
}

/**
 * Returns the position of the leftmost button being pushed.
 * @return the position of the leftmost button being pushed. A 6 is the leftmost button, 1 is the rightmost button. Return 0 if no button is being pressed.
 */
uint8_t button_getButton() {
    int i;
    for (i = 5; i >= 0 ; i--) {
        // Check each bit from bit 5 to bit 0
        if (~GPIO_PORTE_DATA_R & (1 << i)) {
            // Return the first bit that is set to 0
            return i + 1;
        }
    }
    // No bits 0 (no buttons pressed), return 0
    return 0;
}


