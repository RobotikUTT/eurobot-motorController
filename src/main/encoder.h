#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>
#include "io.h"


namespace encoder {
    /**
     * Count left encoder ticks. Reset every DT
     */
    extern volatile long leftTicks;

    /**
     * Count right encoder ticks. Reset every DT
     */
    extern volatile long rightTicks;
}

/**
 * Left encoder chanA interruption routine
 */
void leftAEncInterrupt();

/**
 * Right encoder chanB interruption routine
 */
void rightAEncInterrupt();

/**
 * Left/Right encoder chanA/chanB interruption routine
 */
void bothBEncInterrupt();

/**
 * Reset leftTicks and rightTicks
 */
void resetEncoders();

#endif
