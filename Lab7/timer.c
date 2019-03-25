/*
 * timer.c
 *
 *  Created on: Mar 15, 2019
 *      @author Isaac Rex
 *      Adapted from (and compatible with) Eric Middleton's timer utility
 */

#include "timer.h"

// 65000 gives a countdown time of exactly 65ms TODO: is it 65000 or 64999?
#define MICROS_PER_TICK 64999UL // Number of microseconds in one timer cycle

/**
 * @brief Tracks if the clock is currently running or stopped
 *
 */
uint8_t _running = 0;

/**
 * @brief Tracks the number of milliseconds passed since a call to startClock()
 *
 */
volatile uint32_t _timeout_ticks;

/**
 * @brief Initialize and start the clock at 0. If the clock is
 * already running on a call, reset the time count back to 0. Uses TIMER4.
 *
 */
void timer_startCounter(void) {
    if (!_running) {
        // Turn on clock to TIMER4
        SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R4;

        // Disable TIMER4 for setup
        TIMER4_CTL_R &= ~TIMER_CTL_TAEN;

        // Set as 16-bit timer
        TIMER4_CFG_R = TIMER_CFG_16_BIT;

        // Configure for periodic, countdown mode
        TIMER4_TAMR_R = TIMER_TAMR_TAMR_PERIOD;

        // Set the initial timer value to have a countdown time of exactly
        // 65ms
        TIMER4_TAILR_R = MICROS_PER_TICK; // TODO: is it 65000 or 6499?

        // Ensure the interrupt status register is cleared
        TIMER4_ICR_R |= TIMER_ICR_TATOCINT;

        // Set the prescaler to 15 to give a period of 1us
        TIMER4_TAPR_R = 0x0F;

        // Allow interrupts from TIMER4 timeout
        TIMER4_IMR_R |= TIMER_IMR_TATOIM;

        // Set the priority of the interrupt to 7 (lowest)
        NVIC_PRI17_R |= NVIC_PRI17_INTC_M;

        // Turn on interrupts from TIMER4 (interrupt number 70)
        NVIC_EN2_R |= (1 << 6);

        // Bind the ISR to the interrupt vector
        IntRegister(INT_TIMER4A, timer_clockTickHandler);

        // Start TIMER4 counting
        TIMER4_CTL_R |= TIMER_CTL_TAEN;
    } else { // Reset the timer to 0
        // Stop the timer counting
        TIMER4_CTL_R &= ~TIMER_CTL_TAEN;

        // Reset _timeout_ticks
        _timeout_ticks = 0;

        // Set TIMER4 back to the top
        TIMER4_TAV_R = MICROS_PER_TICK;

        // Restart the timer
        TIMER4_CTL_R |= TIMER_CTL_TAEN;
    }

    _running = 1;
}

/**
 * @brief Stop the clock and free up TIMER4. Resets the value returned by
 * timer_getMillis() and timer_getMicros().
 *
 */
void timer_stopCounter(void) {
    // Disable TIMER4
    TIMER4_CTL_R &= ~TIMER_CTL_TAEN;

    // Reset _timeout_ticks
    _timeout_ticks = 0;

    // Set TIMER4 back to the top
    TIMER4_TAV_R = MICROS_PER_TICK;

    // Turn off clock to TIMER4
    SYSCTL_RCGCTIMER_R &= ~SYSCTL_RCGCTIMER_R5;

    _running = 0;
}

/**
 * @brief Pauses the clock at the current value.
 *
 */
void timer_pauseCounter(void) {
    // Disable TIMER4
    TIMER4_CTL_R &= ~TIMER_CTL_TAEN;

    _running = 0;
}

/**
 * @brief Resumes the clock after a call to pauseClock().
 *
 */
void timer_resumeCounter(void) {
    // Enable TIMER4
    TIMER4_CTL_R |= TIMER_CTL_TAEN;

    _running = 1;
}

/**
 * @brief Returns the number milliseconds that have passed since startClock()
 * was called. Value rolls over after about 49 days.
 *
 * @return unsigned int number of milliseconds since a call to
 * timer_startClock()
 */
unsigned int timer_getMillis(void) {
    // Adding 500 helps account for rounding errors
    return (timer_getMicros() + 500) / 1000;
}

/**
 * @brief Returns the number of microseconds passed since a call to
 * startClock(). Value rolls over after about 71 minutes.
 *
 * @return unsigned int number of microseconds since a call to startClock()
 */
unsigned int timer_getMicros(void) {
    unsigned int ticks;
    unsigned int micros;

    // Disable interrupts while we get the values
    TIMER4_IMR_R &= ~TIMER_IMR_TATOIM;

    // Get the number of microseconds currently in TIMER4
    micros = MICROS_PER_TICK - TIMER4_TAR_R & 0xFFFF;

    if (TIMER4_RIS_R & TIMER_RIS_TATORIS) {
        // If the timer overflows while we're getting the time
        ticks = (_timeout_ticks + 1);
        micros = 0;
    } else {
        ticks = _timeout_ticks;
    }

    // Reenable interrupts from TIMER timeout
    TIMER4_IMR_R |= TIMER_IMR_TATOIM;

    return ticks * MICROS_PER_TICK + micros;
}

/**
 * @brief Pauses the program for the specifeid number of microseconds.
 *
 * @param delay_time number of microseconds to pause for
 */
void timer_waitMicros(unsigned int delay_time) {

    if (delay_time <= 2) {
        // Overhead of the function call is around 1.5us
        return;
    } else {
        delay_time -= 2;
    }

    while (delay_time > 0) { // ldr: 2, cmp: 1, bne: 1; 4 cycles
        // 16 cycles = 1us: need 16 - 9 = 7 NOP cycles
        // Experimentally, 6 is accurate. Missing a cycle?
        asm(" NOP"
            "\n"
            " NOP"
            "\n"
            " NOP"
            "\n"
            " NOP"
            "\n"
            " NOP"
            "\n"
            " NOP");
        delay_time--; // ldr: 2, subs: 1, str: 2; 5 cycles
    }
}

/**
 * @brief Pauses the program for the specified number of milliseconds.
 *
 * @param delay_time number of milliseconds to pause for
 */
void timer_waitMillis(unsigned int delay_time) {
    unsigned int start = timer_getMicros();
    unsigned int current_micros = timer_getMicros();

    while (delay_time > 0) {
        current_micros = timer_getMicros();
        // Use a while loop in case a long ISR is called
        while (delay_time > 0 && ((current_micros - start) >= 1000)) {
            delay_time--;
            start += 1000;
            current_micros = timer_getMicros();
        }
    }
}

/**
 * @brief ISR handler to increment the timeout variable for tracking total
 * milliseconds
 *
 */
static void timer_clockTickHandler() {
    // Clear interrupt flag
    TIMER4_ICR_R |= TIMER_ICR_TATOCINT;
    _timeout_ticks++;
}
