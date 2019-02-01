/*
 * movement.c
 *
 *  Created on: Jan 30, 2019
 *      Author: irex
 */
#include "movement.h"

// Function to return the smaller of two values
#define min(a,b) ((a < b) ? a : b)
#define max(a,b) ((a > b) ? a : b)

typedef enum BumpSide {RIGHT, LEFT}side;
int collision_correct(oi_t *sensor, side bumped);

int move_forward(oi_t *sensor, int centimeters) {
    // Sets the maximum velocity to move forward
    const int end_velocity = 250;
    // Tracks the current velocity (for acceleration)
    int current_velocity = 10;
    // Total distance moved; distance is in mm
    int distance = 0;

    oi_setWheels(current_velocity, current_velocity); // Start moving

        while (distance < centimeters * 10) { // While we haven't moved the required distance
            oi_update(sensor); // update sensors
            distance += (sensor -> distance); // Find out how much we moved since the last loop

            // Bump detection
            if(sensor -> bumpRight) { // If we bumped something on the right
                oi_setWheels(0, 0); // Stop moving
                current_velocity = 10; // Reset the velocity for acceleration
                distance += collision_correct(sensor, RIGHT);
            }
            else if (sensor -> bumpLeft) {
                oi_setWheels(0, 0); // Stop moving
                current_velocity = 10; // Reset the velocity for acceleration
                distance += collision_correct(sensor, LEFT);
            }

            // Acceleration logic
            current_velocity = min(current_velocity + 25, end_velocity); // Increase the velocity by 25 until we reach the maximum
            oi_setWheels(current_velocity, current_velocity);
        }

    oi_setWheels(0, 0); // Stop moving
    return distance / 10.00; // Return the distance moved in cm
}

int turn(oi_t *sensor, int degrees) {
    // Sets the turning velocity
    const int velocity = 95;
    // Factor for adjusting sensor error
    const float error = 0.88;
    // Total amount moved
    int current_degrees = 0;

    // Decide which direction to turn based on sign of degrees
    if (degrees > 0) {
        oi_setWheels(velocity, -velocity);
    }
    else if (degrees < 0) {
        oi_setWheels(-velocity, velocity);
    }

    // Keep turning until we have turned the desired amount
    while (abs(current_degrees) < abs(degrees) * error) {
        oi_update(sensor);
        current_degrees += sensor->angle;
    }

    oi_setWheels(0, 0); // Stop wheels
    return current_degrees; // Return total angle turned
}

// Helper method to correct for a bump. Backs up by 15cm, turns, moves 25 cm, turns back. Returns the amount backed up
int collision_correct(oi_t *sensor, side bumped) {
    // Sets the maximum velocity to move forward
    const int end_velocity = -300;
    // Sets the amount to move backwards
    const int move_back = -150; // 150mm = 15cm
    // Determine if we should turn to the left or right.
    int angle = (bumped == RIGHT ? 90 : -90);
    // Tracks the current velocity (for acceleration)
    int current_velocity = -10;
    int distance = 0;

    oi_setWheels(current_velocity, current_velocity); // Start moving

        while (distance > move_back) { // While we haven't moved the required distance
            oi_update(sensor); // update sensors
            distance += (sensor -> distance); // Find out how much we moved since the last loop

            // Acceleration logic
            current_velocity = max(current_velocity - 25, end_velocity); // Increase the velocity by 25 until we reach the maximum
            oi_setWheels(current_velocity, current_velocity);
        }

        turn(sensor, angle);
        move_forward(sensor, 25);
        turn(sensor, -angle);
        oi_setWheels(0, 0); // Stop moving
        return distance; // Return the amount moved in this helper
}
