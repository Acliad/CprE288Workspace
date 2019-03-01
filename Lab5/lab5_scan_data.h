/*
 * lab5_scan_data.h
 *
 *  Created on: Feb 23, 2019
 *      Author: Sam Stifter
 */

#ifndef LAB5_SCAN_DATA_H_
#define LAB5_SCAN_DATA_H_

// Do not Remove.
#include <stdio.h>

// Do not modify or remove.
#define MAX_DEGREES 180
// Do not modify or remove. 
#define FILE_LINE_PARAMS 3

// Used to hold a reading of IR and Sonar data
struct reading
{
    float ir_distance;
    float sonar_distance;
};

// Pass in the filename (put the file in Debug) and it will read out the
// data from the file and put it in the array you pass in to reading_array.
// The reading_array must be 181 elements large since it reads from 0 to 180.
// file_name should just be a string literal.
// In the reading_array, the data points are stored at the index of the degree
// of the reading. i.e. a 45 degree reading is stored at reading_array[45].
void get_reading(char *file_name, struct reading *reading_array);

#endif /* LAB5_SCAN_DATA_H_ */
