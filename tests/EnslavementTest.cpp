#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <chrono>
#include <thread>

#include "Enslavement.h"
#include "Odometry.h"
#include "Encoder.h"
#include "Motor.h"
#include "./mocks/MockEncoder.h"

using ::testing::AtLeast;
using ::testing::Return;
using ::testing::Invoke;


namespace EnslavementTest
{
    /**
     * Emulate perfect DC motor and encoder,
     * generating linear response to PWM
     */
    
    ACTION_P2(FakeGetTicks, motor, Vmax)
    {
        return Odometry::metersToTicks( (float)(motor->getPWM()) / (float) 255 * (float)Vmax );
    }


    /*
        Params
     */
    
    unsigned long deltaT = 1000; //10^-3s
    double maxAcceleration = 0.05; //m.s^-2
    double maxVelocity = 1; //m.s^-1


    Motor *leftMotor = new Motor(0, 0, 0); //Pin value are not relevant here
    Motor *rightMotor = new Motor(0, 0, 0);

    //NiceMock used to avoid warning of "WillByDefault"
    testing::NiceMock<MockEncoder> leftEncoder(1, 2);
    testing::NiceMock<MockEncoder> rightEncoder(3, 4);

    /* Odometry singleton already exists thanks to OdometryTest
        TODO: better using of singleton pattern
     */
    Odometry *odometry = Odometry::getInst(NULL, NULL);
    Enslavement enslavement(deltaT, maxAcceleration, maxVelocity, 
        leftMotor, rightMotor);


    TEST(Enslavement, straightLine)
    {
        srand(time(NULL));

        odometry->setLeftEncoder(&leftEncoder);
        odometry->setRightEncoder(&rightEncoder);
        odometry->reset();

        enslavement.goTo((CarthesianCoordinates){150, 0});

        for (int i = 0; i < 250; i++)
        {
            ON_CALL(leftEncoder, getTicks())
                .WillByDefault(FakeGetTicks(leftMotor, maxVelocity));
            ON_CALL(rightEncoder, getTicks())
                .WillByDefault(FakeGetTicks(leftMotor, maxVelocity));

            //Test is based on Serial output. Ctrl+c to end. TODO: find better solution
            enslavement.compute();
            //std::this_thread::sleep_for(std::chrono::milliseconds(deltaT));
        }
    }
}
