#include "motor.h"

void runMotor(Motor motor, int value) {
    int dirAPin, dirBPin, pwmPin;

    if (motor == LEFT) {
        dirAPin = MOTOR_L_A;
        dirBPin = MOTOR_L_B;
        pwmPin  = MOTOR_L_PWM;
    }
    else {
        dirAPin = MOTOR_R_A;
        dirBPin = MOTOR_R_B;
        pwmPin  = MOTOR_R_PWM;
    }

    // Move forward
    if (value > 0) {
        digitalWrite(dirAPin, 1);
        digitalWrite(dirBPin, 0);
    }
    // Move backward
    else if (value < 0) {
        digitalWrite(dirAPin, 0);
        digitalWrite(dirBPin, 1);
        value = -value;
    }
    // Brake
    else {
        digitalWrite(dirAPin, 0);
        digitalWrite(dirBPin, 0);
    }

    analogWrite(pwmPin, value);
}

void stopMotor(Motor motor) {
    runMotor(motor, 0);
}
