/*
 * servo.h
 *
 *  Created on: Mar 29, 2019
 *      Author: jmartin3
 */

#ifndef SERVO_H_
#define SERVO_H_

#include <inc/tm4c123gh6pm.h>
#include <stdint.h>
#include <stdbool.h>
#include "driverlib/interrupt.h"

void servo_init(void);

int servo_move(double degrees);


#endif /* SERVO_H_ */
