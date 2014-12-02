//#include "Encoder.h"
//#include "Motor.h"
#include "SerialCom.h"

#define DEBUG


/**
 * Constants
 */

//Encoders
const byte ENCODER_L_A_PIN = 3;
const byte ENCODER_L_B_PIN = 2;
const byte ENCODER_L_I_PIN = 0;

const byte ENCODER_R_A_PIN = 5;
const byte ENCODER_R_B_PIN = 4;
const byte ENCODER_R_I_PIN = 1;


//Motors
const byte MOTOR_L_PIN     = 10;
const byte DIR_L_PIN       = 9;

const byte MOTOR_R_PIN     = 12;
const byte DIR_R_PIN       = 11; //TODO: verify registry timers



//System
const byte LOOP_TIME       = 5; //ms
const byte REDUCTOR_RATIO  = 19;
const byte TICK_PER_SPIN   = 500;


// Encoder *leftEncoder;
// Encoder *rightEncoder;

// Motor *leftMotor;
// Motor *rightMotor;

// unsigned long millis_;
// unsigned long lastMillis;
// unsigned long timeElapsed;

// int kp = 1000;
// int ki = 0;
// int kd = 0; 


SerialCom* serialCom;


// void leftTicks()
// {
//     leftEncoder->listenToTicks();
// }


// void rightTicks()
// {
//     rightEncoder->listenToTicks();
// }


void setup()
{
//     leftEncoder = new Encoder(ENCODER_L_A_PIN, ENCODER_L_B_PIN, ENCODER_L_I_PIN);
//     rightEncoder = new Encoder(ENCODER_R_A_PIN, ENCODER_R_B_PIN, ENCODER_R_I_PIN);

//     leftMotor = new Motor(MOTOR_L_PIN, DIR_L_PIN);
//     rightMotor = new Motor(MOTOR_R_PIN, DIR_R_PIN);

//     //Interrupts
//     attachInterrupt(ENCODER_L_I_PIN, leftTicks, FALLING);
//     attachInterrupt(ENCODER_R_I_PIN, rightTicks, FALLING);


//     //Override PWM frequency
//     int eraser = 7;
//     int prescaler = 1;

//     TCCR1B &= ~eraser;


//     #ifdef DEBUG
//         Serial.begin(9600);
//         Serial.println("Setup finished\n\n");
//     #endif

    serialCom = new SerialCom();
}



void loop()
{
//     millis_ = millis();
//     timeElapsed = millis_ - lastMillis;

//     if ((millis_ - lastMillis) >= LOOP_TIME)
//     {
//         //TODO PID
//     }
    serialCom->setSendCommand(8);
    serialCom->setSendPriority(true);
    serialCom->writeByte(0x42);
    serialCom->send();
}