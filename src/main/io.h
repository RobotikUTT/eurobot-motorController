#ifndef TEST_H
#define TEST_H

#include <Arduino.h>
#include "encoder.h"

#define MOTOR_L_DIR   4
#define MOTOR_L_BRAKE 6
#define MOTOR_L_PWM   5

#define MOTOR_R_DIR   A0
#define MOTOR_R_BRAKE A1
#define MOTOR_R_PWM   3

#define ENCODER_L_A   7
#define ENCODER_L_B   A2

#define ENCODER_R_A   8
#define ENCODER_R_B   A3

/**
 * Initiate input and output GPIO processes
 */
void initIO();

#endif
