#ifndef _TEST_H
#define _TEST_H

#include <Arduino.h>
#include "motor.h"
#include "odometry.h"
#include "encoder.h"
#include "control.h"
#include "PID.h"

/**
 * Read parameters from UART then test motionControl until a goal is reached
 * or timeout
 */
void testMotionControl();

/**
 * Make the two motors run for two seconds in each direction
 */
void testMotors();

/**
 * Send the two encoders ticks by UART
 */
void testEncoder();

#endif
