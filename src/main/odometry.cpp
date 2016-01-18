#include "odometry.h"

double x;
double y;
float theta;


inline void computeOdometry() {
	float dL     = 1/2 * (encoder::leftTicks + encoder::rightTicks),
		  dTheta = encoder::leftTicks - encoder::rightTicks,
		  alpha  = theta + 0.5 * dTheta;

	theta += dTheta;

	x += dL * cos(alpha);
	y += dL * sin(alpha);
}

void resetOdometry() {
	x     = 0;
	y     = 0;
	theta = 0;
}
