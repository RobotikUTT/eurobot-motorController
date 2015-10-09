#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>
#include "io.h"

typedef enum Motor {
    LEFT,
    RIGHT
} Motor_t;

void runMotor(Motor motor, int value);
void stopMotor(Motor motor);

#endif
