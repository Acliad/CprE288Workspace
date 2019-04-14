/**
 * @file proccess_test.c
 * @author Isaac Rex
 * @brief Main for testing the data proccessing repeatably
 * @version 0.1
 * @date 2019-04-14
 *
 *
 */

#include "object_data.h"
#include "lcd.h"
#include <math.h>
#include <stdint.h>

typedef struct object {
  unsigned int start_i;
  unsigned int end_i;
  unsigned int distance;
  unsigned int index;
} object_t;

void proccess_data(object_t destination[], distance_t *data, int length);

int main(void) {
  object_t objects[10] = {0};
  proccess_data(objects, data, 181);

  int num_objects = 0;
  while(objects[num_objects].index != 0) {
      num_objects++;
  }
  
  int i = 0;
  while (1) {
    int object_index = objects[i].index;
    int ang_width = objects[i].end_i - objects[i].start_i;
    float distance = objects[i].distance;
    float linear_width = distance * ang_width * M_PI / 180.00;
    lcd_printf("Object #: %d\nAngular Width: %d\nWidth: %.2f\nDistance: %.2f",
               object_index, ang_width, linear_width, distance);

    i = (i + 1) % num_objects;
    timer_waitMillis(1500);
  }
}

void proccess_data(object_t destination[], distance_t data[], int length) {
  unsigned int object_num = 0;
  unsigned int distance_sum = 0;
  uint8_t processing = 0;

  int i = 0;
  for (i = 0; i < length; i++) {
    if (data[i].ir_distance < 75.00) {
      if (!processing) {
        processing = 1;
        destination[object_num].start_i = i;
        destination[object_num].index = object_num + 1;
      }
      distance_sum += data[i].sonar_distance;
    } else if (processing) {
      processing = 0;
      destination[object_num].end_i = i;
      unsigned int num_samples = i - (destination[object_num].start_i);
      if (num_samples > 0) {
        destination[object_num].distance = distance_sum / (num_samples);
        object_num++;
      }
      distance_sum = 0;
    }
  }
}
