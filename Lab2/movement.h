/*
 * movement.h
 *
 *  Created on: Jan 30, 2019
 *      Author: irex
 */

#ifndef MOVEMENT_H_
#define MOVEMENT_H_

#include "open_interface.h"

int move_forward(oi_t *sensor, int centimeters);
int turn(oi_t *sensor, int degrees);

#endif /* MOVEMENT_H_ */
