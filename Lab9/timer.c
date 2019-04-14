/*
 * timer.c
 *
 *  Created on: Mar 15, 2019
 *      @author Isaac Rex
 *      Adapted from (and compatible with) Eric Middleton's timer utility
 */

// TODO: Check value of MICROS_PER_TICK
// TODO: Change timers back to TIMER5

#include "timer.h"

// 65000 gives a countdown time of exactly 65ms TODO: is it 65000 or 64999?
#define MICROS_PER_TICK 65000UL // Number of microseconds in one timer cycle

/**
 * @brief Tracks if the clock is currently running or stopped
 *
 */
unsigned char _running = 0;

/**
 * @brief Tracks the number of milliseconds passed since a call to startClock()
 *
 */
volatile unsigned int _timeout_ticks;

/**
 * @brief Initialize and start the clock at 0. If the clock is
 * already running on a call, reset the time count back to 0. Uses TIMER4.
 *
 */
void timer_init(void) {
    if (!_running) {
        SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R4; // Turn on clock to TIMER4
        TIMER4_CTL_R &= ~TIMER_CTL_TAEN;           // Disable TIMER4 for setup
        TIMER4_CFG_R = TIMER_CFG_16_BIT;           // Set as 16-bit timer
        TIMER4_TAMR_R = TIMER_TAMR_TAMR_PERIOD;    // Periodic, countdown mode
        TIMER4_TAILR_R = MICROS_PER_TICK - 1;      // Countdown time of 65ms
        TIMER4_ICR_R |= TIMER_ICR_TATOCINT; // Clear timeout interrupt status
        TIMER4_TAPR_R = 0x0F;               // 15 gives a period of 1us
        TIMER4_IMR_R |= TIMER_IMR_TATOIM;   // Allow TIMER4 timeout interrupts
        NVIC_PRI17_R |= NVIC_PRI17_INTC_M;  // Priority 7 (lowest)
        NVIC_EN2_R |= (1 << 6);             // Enable TIMER4 interrupts

        IntRegister(INT_TIMER4A, timer_clockTickHandler); // Bind the ISR
        TIMER4_CTL_R |= TIMER_CTL_TAEN; // Start TIMER4 counting

        _running = 1;
    }
}

/**
 * @brief Stop the clock and free up TIMER4. Resets the value returned by
 * timer_getMillis() and timer_getMicros().
 *
 */
void timer_stop(void) {
    TIMER4_CTL_R &= ~TIMER_CTL_TAEN;            // Disable TIMER4
    _timeout_ticks = 0;                         // Reset tick counter
    TIMER4_TAV_R = MICROS_PER_TICK;             // Set TIMER4 back to the top
    SYSCTL_RCGCTIMER_R &= ~SYSCTL_RCGCTIMER_R5; // Turn off clock to TIMER4
    _running = 0;
}

/**
 * @brief Pauses the clock at the current value.
 *
 */
void timer_pause(void) {
    TIMER4_CTL_R &= ~TIMER_CTL_TAEN; // Disable TIMER4
    _running = 0;
}

/**
 * @brief Resumes the clock after a call to pauseClock().
 *
 */
void timer_resume(void) {
    TIMER4_CTL_R |= TIMER_CTL_TAEN; // Enable TIMER4
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
    unsigned int ticks;
    unsigned int millis;

    TIMER4_IMR_R &= ~TIMER_IMR_TATOIM; // Disable timeout interrupts

    millis = (MICROS_PER_TICK - TIMER4_TAR_R & 0xFFFF) / 1000;
    if (TIMER4_RIS_R & TIMER_RIS_TATORIS) {
        // If the timer overflows while we're getting the time
        ticks = (_timeout_ticks + 1);
        millis = 0;
    } else {
        ticks = _timeout_ticks;
    }

    TIMER4_IMR_R |= TIMER_IMR_TATOIM; // Reenable interrupts from TIMER timeout

    return ticks * (MICROS_PER_TICK / 1000) + millis;
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

    TIMER4_IMR_R &= ~TIMER_IMR_TATOIM; // Disable TIMER4 timeout interrupts

    micros = MICROS_PER_TICK - TIMER4_TAR_R & 0xFFFF;

    if (TIMER4_RIS_R & TIMER_RIS_TATORIS) {
        // If the timer overflows while we're getting the time
        ticks = (_timeout_ticks + 1);
        micros = 0;
    } else {
        ticks = _timeout_ticks;
    }

    TIMER4_IMR_R |= TIMER_IMR_TATOIM; // Reenable TIMER4 interrupts

    return ticks * MICROS_PER_TICK + micros;
}

/**
 * @brief Pauses execution for the specifeid number of microseconds.
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
 * @brief Pauses execution for the specified number of microseconds.
 *
 * @param delay_time number of microseconds to pause for
 */
void timer_waitMillis(unsigned int delay_time) {
    unsigned int start = timer_getMicros();
    unsigned int current_micros = timer_getMicros();

    while (delay_time > 0) {
        current_micros = timer_getMicros();
        // Uses a while loop (instead of if) in case a long ISR is called
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
    TIMER4_ICR_R |= TIMER_ICR_TATOCINT; // Clear interrupt flag
    _timeout_ticks++;
}
