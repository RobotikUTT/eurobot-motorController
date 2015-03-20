#include "Encoder.h"
#include "Motor.h"
#include "SerialCom.h"
#include "Enslavement.h"
#include "Odometry.h"

#define DEBUG


/**
 * Constants
 */

//Encoders
//Channel A is used for the interupt

const byte ENCODER_R_A_PIN = 21;
const byte ENCODER_R_B_PIN = 31;

const byte ENCODER_L_A_PIN = 20;
const byte ENCODER_L_B_PIN = 33;


//Motors
const byte MOTOR_L_PIN = 13;
const byte DIR_L_PIN_A = 24;
const byte DIR_L_PIN_B = 22;

const byte MOTOR_R_PIN = 12;
const byte DIR_R_PIN_A = 26; 
const byte DIR_R_PIN_B = 28; 


Encoder *leftEncoder;
Encoder *rightEncoder;

Motor *leftMotor;
Motor *rightMotor;

Enslavement *enslavement;

SerialCom* serialCom;


void leftTicks()
{
    leftEncoder->listenToTicks();
}

void rightTicks()
{
    rightEncoder->listenToTicks();
}

int getInterruptNumber(int pin)
{
    switch (pin)
    {
        case 2:
            return 0;

        case 3:
            return 1;

        case 21:
            return 2;

        case 20:
            return 3;

        case 19:
            return 4;

        case 18:
            return 5;

        default:
            return -1;
    }
}

void setup()
{
    //Encoders
    leftEncoder = new Encoder(ENCODER_L_A_PIN, ENCODER_L_B_PIN);
    rightEncoder = new Encoder(ENCODER_R_A_PIN, ENCODER_R_B_PIN);

    //Motors
    leftMotor = new Motor(MOTOR_L_PIN, DIR_L_PIN_A, DIR_L_PIN_B);
    rightMotor = new Motor(MOTOR_R_PIN, DIR_R_PIN_A, DIR_R_PIN_B);

    //Init odometry singleton, because enslavement object won't have access to encoders object.
    //This is dirty work, but it works, and it needs to be refactored. :)
    Odometry::getInst(leftEncoder, rightEncoder);

    //Enslavement
    enslavement = new Enslavement(1000, 0.05, 1, leftMotor, rightMotor);

    //Interrupts
    attachInterrupt(getInterruptNumber(ENCODER_L_A_PIN), leftTicks, FALLING);
    attachInterrupt(getInterruptNumber(ENCODER_R_A_PIN), rightTicks, FALLING);


    //Override PWM frequency
     int eraser = 7;
     int prescaler = 1;

     TCCR3B &= ~eraser;

    //Init serial communication
    serialCom = new SerialCom();

    //Ping pong ~ test the communication
    //TODO: serialCom->test
    // serialCom->setSendCommand(SerialComCmd::CMD_TEST);
    // serialCom->writeUInt8(0);
    // serialCom->send();
    //
    Serial.begin(115200);
    enslavement->goTo({1, 0});
}



void loop()
{
    //Read serial packets
    // serialCom->doReadJob();

    //Motor enslavement
    enslavement->compute();
}
