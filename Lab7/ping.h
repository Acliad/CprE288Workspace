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

void ping_init(void);

void ping_read(void);

void ping_delayMicros(unsigned int delay_time);

#endif /* PING_H_ */
