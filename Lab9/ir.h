/*
 * ir.h
 *
 * Header file to set up a basic ADC set of functions
 *
 *  Created on: Mar 8, 2019
 *      Author: jmartin3, irex
 */

#ifndef ADC_H_
#define ADC_H_
#define BIT0 0x01
#define BIT1 0x02
#define BIT2 0x04
#define BIT3 0x08
#define BIT4 0x10
#define BIT5 0x20
#define BIT6 0x40
#define BIT7 0x80
#define PB4 BIT4

#include <inc/tm4c123gh6pm.h>
#include <stdint.h>
#include <stdio.h>

/**
 * @brief Sets up the infrared sensor.
 *
 */
void ir_init(void);

/**
 * @brief Sets the amount of hardware averaging: 2^rate. Max is 6
 *
 */
void ir_setAvg(int avg_rate);

/**
 * @brief Returns the raw value of the ADC connected to PB4 (IR sensor).
 *
 * @return int raw value of PB4
 */
int ir_read(void);

/**
 * @brief Returns the distance read by the IR sensor in cm.
 *
 * @param maximum valid distance for the returned data
 * @return int IR sensor distance in cm
 */
double ir_readCM(double max_dist);

/**
 * @brief Returns the distance read by the IR sensor in cm, averaged avg_times
 * number of times. Returns 0 if read distance is outside range of max_dist
 *
 * @param max_dist maximum valid distance for the returned data 
 * @param avg_times number of samples to average
 * @return double IR sensor distance in cm
 */
double ir_read_avgCM(double max_dist, int avg_times);

#endif /* ADC_H_ */
