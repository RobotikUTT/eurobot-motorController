#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>
#include "io.h"

extern volatile long leftTicks, rightTicks;

/**
 * Left encoder interruption routine
 */
void leftEncInterrupt();

/**
 * Right encoder interruption routine
 * @method rightEncInterrupt
 */
void rightEncInterrupt();

#endif
