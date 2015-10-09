#include "encoder.h"

volatile long leftTicks = 0,
			 rightTicks = 0;

void leftEncInterrupt() {
	// Pin D7 is high
	if(PIND & _BV(PD7)) {
		// Pin A2 is high
		if(PINC & _BV(PC2)) {
			leftTicks++;
		}
		else {
			leftTicks--;
		}
	}
}

void rightEncInterrupt() {
	// Pin D8 is high
	if(PINB & _BV(PB0)) {
		// Pin A3 is high
		if(PINC & _BV(PC3)) {
			rightTicks++;
		}
		else {
			rightTicks--;
		}
	}
}
