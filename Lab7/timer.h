/*
 * timer.h
 *
 *  Created on: Mar 15, 2019
 *      Author: irex
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "driverlib/interrupt.h"
#include <inc/tm4c123gh6pm.h>
#include <stdint.h>

/**
 * @brief Tracks the number of milliseconds passed since a call to startClock()
 *
 */
volatile unsigned int _clock_ticks;

/**
 * @brief Initialize and start the clock at 0. If the clock is
 * already running on a call, reset the time count back to 0. Uses TIMER5.
 *
 */
void timer_startClock(void);

/**
 * @brief Stop the clock and free up TIMER5. Resets the value returned by
 * getMillis() and get Micros().
 *
 */
void timer_stopClock(void);

/**
 * @brief Pauses the clock at the current value.
 *
 */
void timer_pauseClock(void);

/**
 * @brief Resumes the clock after a call to pauseClock().
 *
 */
void timer_resumeClock(void);

/**
 * @brief Returns the number milliseconds that have passed since startClock()
 * was called. Value rolls over after about 49 days.
 *
 * @return unsigned int number of milliseconds since startClock() was called
 */
unsigned int timer_getMillis(void);

/**
 * @brief Returns the number of microseconds passed since startClock() was
 * called. Value rolls over after about 71 minutes. Resolution is about TODO:
 * calculate resoultion
 *
 * @return unsigned int number of microseconds since startClock() was called
 */
unsigned int timer_getMicros(void);

/**
 * @brief Pauses the program for the specified number of milliseconds. Maximum
 * pause length is TODO: calculate
 *
 * @param delay_time number of milliseconds to pause for
 */
void timer_delayMillis(unsigned int delay_time);

/**
 * @brief Pauses the program for the specifeid number of milliseconds. Maximum
 * pause length is TODO: calculate
 *
 * @param delay_time number of microseconds to pause for
 */
void timer_delayMicros(unsigned int delay_time);

/**
 * @brief Sets up an interrupt to call the given function once every given
 * milliseconds. Uses TIMER4 for the countdown. Function f executes inside an
 * ISR, so keep the passed function as short as possible. Maximum interval time
 * is TODO: calculate
 *
 * @param f the function to call
 * @param millis the interval between calls
 */
void timer_fireEvery(void (*f)(void), int millis);

/**
 * @brief Sets up an interrupt to call the given function after the given number
 * of milliseconds. Uses TIMER4 for the countdown, and thus can only be used
 * when timer_fireEvery() is not being used. Function f
 * executes inside an ISR and should be kept as short as possible.
 *
 * @param f the function to call
 * @param millis milliseconds until call
 */
void timer_fireOnce(void (*f)(void), int millis);
/**
 * @brief ISR handler to increment the _clock_ticks variable for tracking
 * milliseconds
 *
 */
static void timer_clockTickHandler();

#endif /* TIMER_H_ */
