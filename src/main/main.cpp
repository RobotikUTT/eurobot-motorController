#include "communication.h"
#include "io.h"
#include "motor.h"
#include "constants.h"
#include "odometry.h"
#include "PID.h"
#include "control.h"
#include "test.h"

namespace main {
	unsigned long lastMillis = 0;
}


/**
 * Entry point. Called once
 */
void setup() {
	initCommunication();
	initIO();

	#ifdef ACTUATOR_TEST
		testMotors();
	#elif CONTROL_TEST

	#else
		// rightVelPID.kP = 0.6;
		// rightVelPID.kI = 0.1;
		// rightVelPID.kD = 0.003;
		rightVelPID.kP = 0;
		rightVelPID.kI = 0;
		rightVelPID.kD = 0;

		leftVelPID.kP  = 0.43;
		leftVelPID.kI  = 0.017;
		leftVelPID.kD  = 0.001;

		control::rightVelGoal = 200;
		control::leftVelGoal  = 400;
	#endif
}

/**
 * Loop routine
 */
void loop() {
	#ifdef ACTUATOR_TEST
		testEncoder();
	#elif CONTROL_TEST
		testMotionControl();
	#else
	    unsigned long now = millis();
	    unsigned int timeElapsed = (now - main::lastMillis);

	    if (timeElapsed >= DT) {
			main::lastMillis = millis();
			motionControl();
		}
	#endif
}
