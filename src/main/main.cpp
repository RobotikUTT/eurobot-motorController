#include <Wire.h>
#include <SPI.h>
#include "mcp_can.h"
#include "CanBus.h"
#include "io.h"
#include "motor.h"
#include "constants.h"


void setup() {
	Serial.begin(115200);
	CanBus.begin(CAN_5KBPS);

	initIO();
}

void loop() {
	unsigned long now = micros();

	long delay = DT * 1000 - (micros() - now);
	if (delay < 0) {
		Serial.println("Iteration too slow");
	}
	else {
		delayMicroseconds(delay);
	}
}
