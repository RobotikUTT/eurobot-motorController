#ifndef PID_H
#define PID_H

typedef struct PID {
	float kP;
	float kI;
	float kD;
	float errorSum;
	float lastInput;
} PID_t;


/**
 * Compute a PID controller output, with derivative
 * on measurement to avoid derivative kick
 * @param  pid        PID controller
 * @param  error      Generally defined by setpoint-input
 * @param  input      Measure
 * @return            Output
 */
float computePID(PID_t* pid, float error, float input);

void resetPID(PID_t* pid);


/**
 * Globally accessible PID structures
 */

// Velocity controller for each motor
extern PID_t leftVelPID, rightVelPID;

// Position controller for each motor
extern PID_t rightPosPID, rightPosPID;
// Polar controllers
extern PID_t distPID, oriPID;

#endif
