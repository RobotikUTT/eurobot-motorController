#ifndef ODOMETRY_H
#define ODOMETRY_H

#include "constants.h"
#include "encoder.h"

#define TICKS_TO_MM(R, ticks) ((float) 2.0*M_PI*R*ticks/TICKS_PER_SPIN)
#define MM_TO_TICKS(R, mm) ((float) TICKS_PER_SPIN/(2.0*M_PI*R*mm))

/**
 * Carthesian position
 */
extern double x, y;

/**
 * Orientation
 */
extern float theta;


/**
 * Update (x, y, theta)
 */
inline void computeOdometry();

/**
 * Reset odometry
 */
void resetOdometry();

#endif