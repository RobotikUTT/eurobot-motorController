
#include "Encoder.h"
#include "Motor.h"
#include "Communication.h"
#include "Enslavement.h"
#include "Odometry.h"

#define DEBUG


/**
 * Constants
 */

//Encoders
//Channel A is used for the interupt

const byte ENCODER_R_A_PIN = 21;
const byte ENCODER_R_B_PIN = 22;

const byte ENCODER_L_A_PIN = 20;
const byte ENCODER_L_B_PIN = 24;


//Motors
const byte MOTOR_L_PIN = 12;
const byte DIR_L_PIN_A = 30;
const byte DIR_L_PIN_B = 32;

const byte MOTOR_R_PIN = 11;
const byte DIR_R_PIN_A = 26;
const byte DIR_R_PIN_B = 28;


Encoder *leftEncoder;
Encoder *rightEncoder;

Motor *leftMotor;
Motor *rightMotor;

Enslavement *enslavement;


void leftTicks()
{
    leftEncoder->listenToTicks();
    // Debug
    // Serial.print("left: ");
    // Serial.println(leftEncoder->getTicks());
}

void rightTicks()
{
    rightEncoder->listenToTicks();
    // Debug
    // Serial.print("right: ");
    // Serial.println(rightEncoder->getTicks());
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
    enslavement = new Enslavement(2, 0.5, 1, leftMotor, rightMotor);

    //Interrupts
    attachInterrupt(getInterruptNumber(ENCODER_L_A_PIN), leftTicks, FALLING);
    attachInterrupt(getInterruptNumber(ENCODER_R_A_PIN), rightTicks, FALLING);


    //Override PWM frequency TODO WARN : Doesn't work on nano
     // int eraser = 7;
     // int prescaler = 1;

     // TCCR3B &= ~eraser;

    // enslavement->goTo(5, 0, false);
    //enslavement->turn(360);

    //Debug
    Serial.begin(115200);
    Serial.println("Ready !");

    //Communication configuration
    Communication::open(0x02);

}



void loop()
{
    //Read serial packets
    // serialCom->doReadJob();
    //Motor enslavement
    //enslavement->compute();
}
