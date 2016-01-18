#include "test.h"


void testMotionControl() {
	for (;;) {
		stopMotor(LEFT);
		stopMotor(RIGHT);

		float params[6];
		float goals[2];

		Serial.println("Enter control params:");
		int i = 0;
		while (i < 6) {
			// Wait for data
			if (Serial.available()) {
				params[i] = Serial.parseFloat();
				i++;
			}
		}

		Serial.println("Enter control type:");
		// Wait for data
		while (!Serial.available()) { }
		control::actualControl = (Control_t) Serial.parseInt();

		Serial.println("Enter goals:");
		i = 0;
		while (i < 2) {
			// Wait for data
			if (Serial.available()) {
				goals[i] = Serial.parseFloat();
				i++;
			}
		}

		resetPID(&leftVelPID);
		resetPID(&rightVelPID);
		resetEncoders();

		leftVelPID.kP  = params[0];
		leftVelPID.kI  = params[1];
		leftVelPID.kD  = params[2];
		rightVelPID.kP = params[3];
		rightVelPID.kI = params[4];
		rightVelPID.kD = params[5];

		control::leftVelGoal  = goals[0];
		control::rightVelGoal = goals[1];

		bool goalReached         = false;
		bool timeout             = false;
		unsigned long start      = millis();
		unsigned long lastMillis = 0;

		do {
		    unsigned long now = millis();
		    unsigned int timeElapsed = (now - lastMillis);

			if (timeElapsed >= DT) {
				lastMillis = now;
		    	velocityControl();
				resetEncoders();
			}

			if (now >= start + 15000) {
				timeout = true;
			}
		} while((goalReached == false) && (timeout == false));

		Serial.println("Test finished");
	}
}

void testMotors() {
	Serial.println("Running left motor...");
	runMotor(LEFT, 155);
	delay(2000);
	runMotor(LEFT, -155);
	delay(2000);
	stopMotor(LEFT);

	Serial.println("Running right motor...");
	runMotor(RIGHT, 155);
	delay(2000);
	runMotor(RIGHT, -155);
	delay(2000);
	stopMotor(RIGHT);
}

void testEncoder() {
	// Send input for 15 seconds
	for(int i = 0; i < 15; i++) {
		Serial.print("l = ");
		Serial.print(encoder::leftTicks);
		Serial.print(" r = ");
		Serial.println(encoder::rightTicks);
		delay(1000);
	}
}
