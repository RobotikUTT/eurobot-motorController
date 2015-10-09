#include "PID.h"

PID_t leftPosPID, rightPosPID;
PID_t leftVelPID, rightVelPID;
PID_t distPID, oriPID;

float PIDCompute(PID_t* pid, float error, float input) {
	/**
	 * dError = dSetpoint - dInput
	 * setPoint constant <=> dError = -dInput
	 * 		             <=> dError = lastInput - input
	 */
	float dTerm = pid->lastInput - input;

	// Numerical integration
	pid->errorSum += error;

	// Output calculation
	return pid->kP*error + pid->kI*pid->errorSum + pid->kD*dTerm;
}