#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>


enum Direction
{
    FORWARD,
    BACKWARD
};


/**
 * DC motor.
 * Provide an API to control a DC motor using L298
 * dual H-bridge.
 */
class Motor
{
    public:

        /**
         * May be used if rotary encoder is usef directly on
         * the motor shaft
         */
        static const byte REDUCTOR_RATIO = 48;

        /**
         * Wheel radius (m)
         */
        static const double WHEEL_RADIUS;

        /**
         * Constructor
         * @param pwmPin  PWM pin
         * @param dirPinA First direction pin
         * @param dirPinB Second direction pin
         */
        Motor(byte pwmPin, byte dirPinA, byte dirPinB);

        /**
         * Destructor
         */
        ~Motor();


        /**
         * Run on a PWM 0<PWM<255
         * 255 is full speed, 0 is immobile.
         * If negative value is given, run backward.
         * @param PWM Voltage average value
         */
        void run(int PWM);

        /**
         * Brake
         */
        void stop();


        /**
         * PWM getter
         * @return PWM
         */
        int getPWM();

        /**
         * Set the minimum PWM value. If the pwm command is < then pwm=0
         * @param pwm - The minimum pwm value
         */
        void setMinPWM(byte pwm);


    protected:

        /**
         * Make the motor run forward or backward
         * @param dir Direction
         */
        void setDir(Direction dir);

        byte PWM;
        Direction dir;
        byte pwmPin;
        byte dirPinA;
        byte dirPinB;
        byte minPWM;
};

#endif