#include "encoder.h"

volatile long encoder::leftTicks  = 0;
volatile long encoder::rightTicks = 0;

using namespace encoder;

volatile byte lastPINC = 0;

void leftAEncInterrupt() {
	// Pin D7 is high
	if(PIND & (_BV(PD7))) {
		// Pin A2 is high
		if(PINC & _BV(PC2)) {
			leftTicks--;
		}
		else {
			leftTicks++;
		}
	}
	else {
		// Pin A2 is high
		if(PINC & _BV(PC2)) {
			leftTicks++;
		}
		else {
			leftTicks--;
		}
	}
}

void rightAEncInterrupt() {
	// Pin D8 is high
	if(PINB & _BV(PB0)) {
		// Pin A3 is high
		if(PINC & _BV(PC3)) {
			rightTicks--;
		}
		else {
			rightTicks++;
		}
	}
	else {
		// Pin A3 is high
		if(PINC & _BV(PC3)) {
			rightTicks++;
		}
		else {
			rightTicks--;
		}
	}
}

void bothBEncInterrupt() {
	// If the pin changed is A2
	if((lastPINC ^ PINC) & _BV(PC2)) {
		// Pin D7 is high
		if(PIND & (_BV(PD7))) {
			// Pin A2 is high
			if(PINC & _BV(PC2)) {
				leftTicks++;
			}
			else {
				leftTicks--;
			}
		}
		else {
			// Pin A2 is high
			if(PINC & _BV(PC2)) {
				leftTicks--;
			}
			else {
				leftTicks++;
			}
		}
	}
	// If the pin changed is A3
	else {
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
		else {
			// Pin A3 is high
			if(PINC & _BV(PC3)) {
				rightTicks--;
			}
			else {
				rightTicks++;
			}
		}
	}

	// Save the last Port C values
	lastPINC = PINC;
}

void resetEncoders() {
	leftTicks  = 0;
	rightTicks = 0;
}
