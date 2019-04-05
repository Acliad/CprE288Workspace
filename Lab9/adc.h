/*
 * adc.h
 *
 * Header file to set up a basic ADC set of functions
 *
 *  Created on: Mar 8, 2019
 *      Author: jmartin3, irex
 */

#ifndef ADC_H_
#define ADC_H_
#define BIT0        0x01
#define BIT1        0x02
#define BIT2        0x04
#define BIT3        0x08
#define BIT4        0x10
#define BIT5        0x20
#define BIT6        0x40
#define BIT7        0x80

#include <stdio.h>
#include <stdint.h>
#include <inc/tm4c123gh6pm.h>

void adc_init(void);
void adc_setAvg(int);
int adc_read(void);

#endif /* ADC_H_ */
