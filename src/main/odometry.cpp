#include "odometry.h"

double x, y;
float theta, previousTheta;

inline void computeOdometry() {
	float dL     = 1/2 * (leftTicks + rightTicks),
		  dTheta = leftTicks - rightTicks;

}

void resetOdometry() {
	x             = 0;
	y             = 0;
	theta         = 0;
	previousTheta = 0;
}