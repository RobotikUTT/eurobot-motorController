#ifndef CONTROL_H
#define CONTROL_H

#include <Arduino.h>
#include "PID.h"
#include "constants.h"
#include "encoder.h"
#include "motor.h"

typedef enum Control {
    POSITION,          // Control motors position
    SPEED,             // Control motors speed
    DISTANCE,          // Control robot distance
    ORIENTATION,       // Control robot orientation
    POLAR              // Control robot distance and orientation
} Control_t;

namespace control {
    /**
     * Velocity goal for left motor in ticks/DT
     */
    extern float leftVelGoal;

    /**
     * Velocity goal for right motor in ticks/DT
     */
    extern float rightVelGoal;

    /**
     * Type of control currently used
     */
    extern Control_t actualControl;
}

/**
 * Loop routine to control either two motors motion (position and speed)
 * or two motors polar coordinates (position and speed)
 */
void motionControl();

/**
 * Control the velocity of two motors
 */
void velocityControl();

/**
 * Filter velocity input to generate a trapzeoïd velocity profile
 * @param  input     Velocity input
 * @param  lastInput Last velocity input
 * @return           Filtered velocity
 */
inline float velocityProfile(float input, float lastInput);

/**
 * Control the position of two motors
 * @return A velocity goal
 */
float positionControl();

/**
 * Control the mean distance made by two motors
 * @return A velocity goal
 */
float distanceControl();

/**
 * Control the mean orientation made by two motors
 * @return A velocity goal
 */
float orientationControl();

#endif
