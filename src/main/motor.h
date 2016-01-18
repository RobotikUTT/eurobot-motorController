#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>
#include "io.h"

typedef enum Motor {
    LEFT,
    RIGHT
} Motor_t;

/**
 * Run a motor at a given speed and direction
 * @param motor Motor to run
 * @param value From -255 to 255 included. Sign is direction, value is speed
 */
void runMotor(Motor motor, int value);

/**
 * Break a motor. Alias to runMotor(motor, 0);
 * @param motor Motor to break
 */
void stopMotor(Motor motor);

#endif
