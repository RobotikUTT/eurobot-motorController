#include "test.h"

void testMotor() {
	Serial.println("Running left motor...");
	runMotor(LEFT, 255);
	delay(2000);
	runMotor(LEFT, -255);
	delay(2000);
	stopMotor(LEFT);

	Serial.println("Running right motor...");
	runMotor(RIGHT, 255);
	delay(2000);
	runMotor(RIGHT, -255);
	delay(2000);
	stopMotor(RIGHT);
}

void testEncoder() {
	for(;;) {
		Serial.print((PIND & _BV(PD7)) > 0);
		Serial.print((PINC & _BV(PC2)) > 0);
		Serial.print("  ");
		Serial.print((PINB & _BV(PB0)) > 0);
		Serial.println((PINC & _BV(PC3)) > 0);
	}
}
