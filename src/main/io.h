#ifndef TEST_H
#define TEST_H

#include <Arduino.h>
#include "encoder.h"

#define MOTOR_L_A   A0
#define MOTOR_L_B   A1
#define MOTOR_L_PWM 3

#define MOTOR_R_A   4
#define MOTOR_R_B   6
#define MOTOR_R_PWM 5

#define ENCODER_L_A 7
#define ENCODER_L_B A2

#define ENCODER_R_A 8
#define ENCODER_R_B A3

void initIO();

#endif
