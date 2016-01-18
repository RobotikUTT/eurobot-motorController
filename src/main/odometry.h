#ifndef ODOMETRY_H
#define ODOMETRY_H

#include "constants.h"
#include "encoder.h"

#define TICKS_TO_MM(ticks) ((float) 2.0*M_PI*R*ticks/TICKS_PER_REV)
#define MM_TO_TICKS(mm) ((float) mm*TICKS_PER_SPIN/(2.0*M_PI*R))

namespace odometry {
    /**
     * Carthesian position
     */
    extern double x;
    extern double y;

    /**
     * Orientation
     */
    extern float theta;
}


/**
 * Update (x, y, theta)
 */
inline void computeOdometry();

/**
 * Reset odometry
 */
void resetOdometry();

#endif
