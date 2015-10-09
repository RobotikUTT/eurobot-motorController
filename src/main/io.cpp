#include "io.h"

void initIO() {
    pinMode(MOTOR_L_A, OUTPUT);
	pinMode(MOTOR_L_B, OUTPUT);
	pinMode(MOTOR_L_PWM, OUTPUT);

	pinMode(MOTOR_R_A, OUTPUT);
	pinMode(MOTOR_R_B, OUTPUT);
	pinMode(MOTOR_R_PWM, OUTPUT);

	pinMode(ENCODER_L_A, INPUT);
	pinMode(ENCODER_L_B, INPUT);

	pinMode(ENCODER_R_A, INPUT);
	pinMode(ENCODER_R_B, INPUT);

	attachInterrupt(ENCODER_L_A, leftEncInterrupt, RISING);
	attachInterrupt(ENCODER_R_A, rightEncInterrupt, RISING);

    // PCINT Activation
    PCICR |= (1 << PCIE0); // PCINT on B port (D8-D13)
    PCICR |= (1 << PCIE2); // PCINT on D port (D0-D7)
    // B port (PCIE0 => PCMSK0) :
    PCMSK0 = (1 << PCINT0); // PCINT on pin 8
    // D port (PCIE2 => PCMSK2) :
    PCMSK2 = (1 << PCINT23); // PCINT on pin 7
}


// Interruptions
ISR(PCINT0_vect) {
	rightEncInterrupt();
}

ISR(PCINT2_vect)
{
    leftEncInterrupt();
}
