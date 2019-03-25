/*
 * ping.h
 *
 *  Created on: Mar 15, 2019
 *      Author: jmartin3
 */

#ifndef PING_H_
#define PING_H_

#include <inc/tm4c123gh6pm.h>
#include <stdint.h>
#include <stdbool.h>
#include "timer.h"
#include "driverlib/interrupt.h"

//TODO: Remove
extern volatile unsigned int num_overflows;


void ping_init(volatile unsigned int *ptr);

void ping_read(void);

void ping_captureHandler(void);

#endif /* PING_H_ */
