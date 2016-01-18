#include "control.h"

float control::leftVelGoal  = 0;
float control::rightVelGoal = 0;

Control_t control::actualControl = SPEED;

unsigned long lastMillis = 0;

void motionControl() {
    switch (control::actualControl) {
        case POSITION: {
            // TODO
            break;
        }

        case SPEED: {
        	velocityControl();
            break;
        }

        case DISTANCE: {
            // TODO
            break;
        }

        case ORIENTATION: {
            // TODO
            break;
        }

        case POLAR: {
            // TODO
            break;
        }
    }

    resetEncoders();
}

void velocityControl() {
    Serial.println(control::leftVelGoal);
    Serial.println(encoder::leftTicks);
    Serial.println(control::rightVelGoal);
    Serial.println(encoder::rightTicks);

    float leftError   = control::leftVelGoal - encoder::leftTicks;
    float rightError  = control::rightVelGoal - encoder::rightTicks;

	float leftOutput  = computePID(&leftVelPID, leftError, encoder::leftTicks);
	float rightOutput = computePID(&rightVelPID, rightError, encoder::rightTicks);

    runMotor(LEFT, leftOutput);
    runMotor(RIGHT, rightOutput);
}

inline float velocityProfile(float input, float lastInput) {
    // TODO
    return input;
}
