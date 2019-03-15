/*
 * timer.h
 *
 *  Created on: Mar 15, 2019
 *      Author: irex
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>
#include <inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"

void timer_startClock(void);

void timer_stopClock(void);

void timer_getMillis(void);

void timer_getMicros(void);

void timer_delayMillis(unsigned int delay_time);

void timer_delayMicros(unsigned int delay_time);


#endif /* TIMER_H_ */
