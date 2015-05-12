
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
const byte ENCODER_R_A_PIN = 3;
const byte ENCODER_R_B_PIN = 12;

const byte ENCODER_L_A_PIN = 2;
const byte ENCODER_L_B_PIN = 4;


//Motors
const byte MOTOR_R_PIN = 5;
const byte DIR_R_PIN_A = 10;
const byte DIR_R_PIN_B = 9;

const byte MOTOR_L_PIN = 6;
const byte DIR_L_PIN_A = 8;
const byte DIR_L_PIN_B = 7;

//Motors config
const byte MOTOR_L_MINPWM = 20;
const byte MOTOR_R_MINPWM = 20;


Encoder *leftEncoder;
Encoder *rightEncoder;

Motor *leftMotor;
Motor *rightMotor;

Enslavement *enslavement;
Odometry *odometry;

unsigned long previous = 0;
int orientation = 0;

void leftTicks()
{
    leftEncoder->listenToTicks();
    // Debug
    //Serial.print("left: ");
    //Serial.println(leftEncoder->getTicks());
}

void rightTicks()
{
    rightEncoder->listenToTicks();
    // Debug
    //Serial.print("right: ");
    //Serial.println(rightEncoder->getTicks());
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
    leftMotor->setMinPWM(MOTOR_L_MINPWM);
    rightMotor = new Motor(MOTOR_R_PIN, DIR_R_PIN_A, DIR_R_PIN_B);
    rightMotor->setMinPWM(MOTOR_R_MINPWM);

    //Init odometry singleton, because enslavement object won't have access to encoders object.
    //This is dirty work, but it works, and it needs to be refactored. :)
    odometry = Odometry::getInst(leftEncoder, rightEncoder);

    //Enslavement
    enslavement = new Enslavement(50, 0.3, 1, leftMotor, rightMotor);

    //Interrupts
    attachInterrupt(getInterruptNumber(ENCODER_L_A_PIN), leftTicks, FALLING);
    attachInterrupt(getInterruptNumber(ENCODER_R_A_PIN), rightTicks, FALLING);


    //Override PWM frequency TODO WARN : Doesn't work on nano
     // int eraser = 7;
     // int prescaler = 1;

     // TCCR3B &= ~eraser;

    // enslavement->goTo(5, 0, false);
    // enslavement->turn(180);

    //Debug
    Serial.begin(115200);
    // Serial.println("Debug ready !");

    //Communication configuration
    Communication::open(0x02, A3, 0xff);
    Communication::setMotors(leftMotor, rightMotor);
    Communication::setEncoders(leftEncoder, rightEncoder);
    Communication::setEnslavement(enslavement);
    Communication::setOdometry(odometry);
}



void loop()
{
    // Serial.println((leftEncoder->getTicks() - rightEncoder->getTicks())/(2*M_PI));
    // odometry->update();
    // CarthesianCoordinates coordinates = odometry->getCoordinates();
    // Serial.println(sqrt(pow(coordinates.x, 2) + pow(coordinates.y, 2)));
    // rightMotor->run(255);
    // Serial.print("left: ");
    // Serial.println(leftEncoder->getTicks());
    // Serial.print("right: ");
    // Serial.println(rightEncoder->getTicks());
    /* CarthesianCoordinates coordinates = odometry->getCoordinates();

     Serial.print("x: ");
     Serial.print(coordinates.x);
     Serial.print(" ");
     Serial.print("y: ");
     Serial.println(coordinates.y);
       leftEncoder->resetTicks();
     rightEncoder->resetTicks(); */


    //     Serial.println(odometry->getOrientation());
    //     leftEncoder->resetTicks();
    //     rightEncoder->resetTicks();
    // delay(1000);



    //Read serial packets
    //Motor enslavement
    enslavement->compute();
}
