/*
 * ir.c
 *
 * Set of function for setting up and reading from the ADC module
 *  Created on: Mar 8, 2019
 *      Author: jmartin3, irex
 */

#include "ir.h"

float coeff_A = 1.217e8;
float coeff_B = -2.143;
float y_intercept = 5;

/**
 * @brief Sets up the infrared sensor.
 *
 */
void ir_init(void) {

  // Turn on the clock for GPIO Port B
  SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;
  // Turn on the clock for ADC0
  SYSCTL_RCGCADC_R |= BIT0;

  // Enable alternate function for GPIO PB4
  GPIO_PORTB_AFSEL_R |= PB4;

  // Configure the pin PB4 for input and analog mode
  GPIO_PORTB_DIR_R &= ~PB4;
  GPIO_PORTB_DEN_R &= ~PB4;
  GPIO_PORTB_AMSEL_R |= PB4;

  // Don't use PB4 as the trigger source, use the program
  GPIO_PORTB_ADCCTL_R &= ~PB4;

  // Disable SS0, sample sequencer 0, for configuration
  ADC0_ACTSS_R &= ~BIT0;

  // Set SS0 to trigger from ADCPSSI
  ADC0_EMUX_R &= 0xFFF0;

  // Sample channel 10
  ADC0_SSMUX0_R |= 0xA;

  // Clear sample sequence control register
  ADC0_SSCTL0_R = 0x0;

  // Set sample sequencer to end and flag after first sample
  ADC0_SSCTL0_R |= 0x6;

  // Re-enable ADC0 SS0
  ADC0_ACTSS_R |= 0x1;
}

/**
 * @brief Sets the amount of hardware averaging: 2^rate. Max is 6
 *
 */
void ir_setAvg(int avg_rate) {
  if (avg_rate < 7) {
    ADC0_SAC_R = avg_rate;
  }
}

/**
 * @brief Returns the raw value of the ADC connected to PB4 (IR sensor).
 *
 * @return int raw value of PB4
 */
int ir_read(void) {

  // Initiate SS0 conversion sequence
  ADC0_PSSI_R = 0x1;

  // Wait for SS0 ADC conversions to complete
  while (~ADC0_RIS_R & BIT0) {
  }

  // Clear the raw interrupt status flag for SS0
  ADC0_ISC_R = BIT0;

  // Return channel 10 from FIFO buffer
  return ADC0_SSFIFO0_R;
}

/**
 * @brief Returns the distance read by the IR sensor in cm.
 *
 * @return int IR sensor distance in cm
 */
double ir_readCM(double max_dist) {
  // Map the timer value to distance
  double distance = (coeff_A)*pow(ir_read(), coeff_B) + y_intercept;

  // Return 0 if distance is outside usable range of IR sensor
  return (distance > max_dist ? max_dist : distance);
}

/**
 * @brief Returns the distance read by the IR sensor in cm, averaged avg_times
 * number of times. Returns 0 if read distance is outside range of max_dist
 *
 * @param max_dist maximum valid distance for the returned data
 * @param avg_times number of samples to average
 * @return double IR sensor distance in cm
 */
double ir_read_avgCM(double max_dist, int avg_times) {
  double total = 0;
  
  int i;
  for (i = 0; i < avg_times; i++) {
    total += ir_readCM(max_dist);
  }

  return total/((float) avg_times);
}
