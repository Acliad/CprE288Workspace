/*
 * timer.c
 *
 *  Created on: Mar 15, 2019
 *      Author: Isaac Rex
 *      Adapted from Eric Middleton's library
 */

#include "timer.h"

// 6500 gives a countdown time of exactly 65ms; TODO: is it 6500 or 6499?
#define TIMER5_INITIAL_VALUE 6500UL
#define MICROS_PER_TICK 65000UL // Number of microseconds in one timer cycle

/**
 * @brief Tracks if the clock is currently running or stopped
 * 
 */
u_int8_t _running = 0;

/**
 * @brief Initialize and start the clock at 0. If the clock is
 * already running on a call, reset the time count back to 0. Uses TIMER5.
 *
 */
void timer_startClock(void)
{

    if (!running)
    {
        // Turn on clock to TIMER5
        SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R5;

        // Disable TIMER5 for setup
        TIMER5_CTL_R &= ~TIMER_CTL_TAEN;

        // Configure for periodic, coundown mode
        TIMER5_CTL_R = TIMER_TAMR_TAMR_PERIOD;

        // Set the prescaler to give a period of 1us
        TIMER5_TAPR_R = 16;

        // Set the initial timer value. 6500 gives a countdown time of exactly 65ms
        TIMER5_TAILR_R = TIMER5_INITIAL_VALUE; // TODO: is it 6500 or 6499?

        // Allow interrupts from TIMER5 timeout
        TIMER5_IMR_R |= TIMER_IMR_TATOIM;

        // Ensure the interrupt status register is cleared
        TIMER5_ICR_R |= TIMER_ICR_TATOCINT;

        // Set the priority of the interrupt to 7 (lowest)
        NVIC_PRI23_R |= NVIC_PRI23_INTA_M;

        // Turn on interrupts from TIMER5 (interrupt number 92)
        NVIC_EN2_R |= (1 << 28);

        // Bind the ISR to the interrupt vector
        IntRegister(INT_TIMER5A, timer_clockTickHandler);

        // Start TIMER5 counting
        TIMER5_CTL_R |= TIMER_CTL_TAEN;
    }
    // Reset the timer to 0
    else
    {
        // Stop the timer counting
        TIMER5_CTL_R &= ~TIMER_CTL_TAEN;

        // Reset _clock_ticks
        _clock_ticks = 0;

        // Set TIMER5 back to the top
        TIMER5_TAV_R = TIMER5_INITIAL_VALUE;

        // Restart the timer
        TIMER5_CTL_R |= TIMER_CTL_TAEN;
    }

    _running = 1;
}

/**
 * @brief Stop the clock and free up TIMER5. Resets the value returned by
 * getMillis() and get Micros().
 *
 */
void timer_stopClock(void)
{
    // Disable TIMER5
    TIMER5_CTL_R &= ~TIMER_CTL_TAEN;

    // Reset _clock_ticks
    _clock_ticks = 0;

    // Set TIMER5 back to the top
    TIMER5_TAV_R = TIMER5_INITIAL_VALUE;

    // Turn off clock to TIMER5
    SYSCTL_RCGCTIMER_R &= ~SYSCTL_RCGCTIMER_R5;

    _running = 0;
}

/**
 * @brief Pauses the clock at the current value.
 *
 */
void timer_pauseClock(void)
{
    // Disable TIMER5
    TIMER5_CTL_R &= ~TIMER_CTL_TAEN;

    _running = 0;
}

/**
 * @brief Resumes the clock after a call to pauseClock().
 *
 */
void timer_resumeClock(void)
{
    // Enable TIMER5
    TIMER5_CTL_R |= TIMER_CTL_TAEN;

    _running = 1;
}

//TODO: Add brief
unsigned int timer_getMillis(void)
{
    // TODO: handle the case where interrupt could trigger during read of _clock_ticks
    // Find total number of milliseconds by multiplying the number of milliseconds
    // per timout event by the number of timout events (ms * num_events); add this
    // to the number of milliseconds currently in the timer (TIMER5_TAR_R–need
    // just the lower 16 bits); add this to 0.5 to correct for rounding errors
    return (MICROS_PER_TICK * _clock_ticks) / 1000.00 +
           (TIMER5_TAR_R & 0xFFFF) / 1000.00 + 0.5;
}

//TODO: Add brief
unsigned int timer_getMicros(void)
{
    return MICROS_PER_TICK * _clock_ticks + TIMER5_TAR_R & 0xFFFF;
}

//TODO: Add brief
void timer_delayMillis(unsigned int delay_time)
{
    if (_running)
    {
        unsigned int start_time = timer_getMillis();
        while (timer_getMillis() - start_time < delay_time)
        {
        }
    }
    else
    {
        // Save the current state of the clock in case it's just paused
        unsigned int old_timer_val = TIMER5_TAV_R;
        unsigned int old_clock_ticks = _clock_ticks;

        // Start the clock, do the delay, stop the clock
        timer_startClock();
        while (timer_getMillis() < delay_time)
        {
        }
        timer_stopClock();

        // Reset the state of the clock to before the delay
        TIMER5_TAV_R = old_timer_val;
        _clock_ticks = old_clock_ticks;
    }
}

