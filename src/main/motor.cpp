#include "motor.h"

void runMotor(Motor motor, int value) {
    int dirPin, brakePin, pwmPin;

    if (motor == LEFT) {
        dirPin   = MOTOR_L_DIR;
        brakePin = MOTOR_L_BRAKE;
        pwmPin   = MOTOR_L_PWM;
    }
    else {
        dirPin   = MOTOR_R_DIR;
        brakePin = MOTOR_R_BRAKE;
        pwmPin   = MOTOR_R_PWM;
    }

    // Brake
    if (value == 0) {
        digitalWrite(brakePin, LOW);
        digitalWrite(pwmPin, HIGH);
        return;
    }

    // Disable brake
    digitalWrite(brakePin, HIGH);

    // Move forward
    if (value > 0) {
        if (value > 255) {
            value = 255;
        }
        digitalWrite(dirPin, HIGH);
    }
    // Move backward
    else if (value < 0) {
        if (value < -255) {
            value = -255;
        }
        digitalWrite(dirPin, LOW);
        value = -value;
    }

    analogWrite(pwmPin, value);
}

void stopMotor(Motor motor) {
    runMotor(motor, 0);
}
