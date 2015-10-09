#include "test.h"

void testMotor() {
	Serial.println("Running left motor...");
	runMotor(LEFT, 255);
	delay(1000);

	Serial.println("Running right motor...");
	runMotor(RIGHT, 255);
}

void testEncoder() {
    Serial.println("Listening to encoders...");

	for (int i = 0; i < 2000; i++) {
		Serial.print("x = ");
		Serial.print(leftTicks);
		Serial.print(" y = ");
		Serial.println(rightTicks);
		delay(1);
	}
}
