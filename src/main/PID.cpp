#include "PID.h"
#include <Wire.h>
#include <SPI.h>

PID_t leftPosPID, rightPosPID;
PID_t leftVelPID, rightVelPID;
PID_t distPID, oriPID;


float computePID(PID_t* pid, float error, float input) {
	/**
	 * dError = dSetpoint - dInput
	 * setPoint constant <=> dError = -dInput
	 * 		             <=> dError = lastInput - input
	 */
	float dTerm = pid->lastInput - input;
	pid->lastInput = input;

	if (pid->errorSum == 0) {
	}
	// Numerical integration
	pid->errorSum += error;

	// Output calculation
	return pid->kP*error + pid->kI*pid->errorSum + pid->kD*dTerm;
}

void resetPID(PID_t* pid) {
	pid->lastInput = 0;
	pid->errorSum  = 0;
}
