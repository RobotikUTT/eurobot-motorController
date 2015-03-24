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
    double fakeGetTicks(Motor* motor, Enslavement* enslavement, double Vmax)
    {
        double ticks = Odometry::metersToTicks( (float)(motor->getPWM()) / (float) 255 * (float)Vmax );
        return ticks;
    }

    /*
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


    TEST(Enslavement, backward)
    {
        odometry->setLeftEncoder(&leftEncoder);
        odometry->setRightEncoder(&rightEncoder);
        odometry->reset();
        odometry->setCoordinates(0, 0);

        enslavement.goTo(-25, 0, false);

        for (int i = 0; i < 46; i++)
        {
            ON_CALL(leftEncoder, getTicks())
                .WillByDefault(Return(fakeGetTicks(leftMotor, &enslavement, maxVelocity)));
            ON_CALL(rightEncoder, getTicks())
                .WillByDefault(Return(fakeGetTicks(rightMotor, &enslavement, maxVelocity)));

            //Test is based on Serial output. Ctrl+c to end. TODO: find better solution
            enslavement.compute();
            //std::this_thread::sleep_for(std::chrono::milliseconds(deltaT));
        }
    }

    TEST(Enslavement, forward)
    {
        enslavement.goTo(0,0,false);

        for (int i = 0; i < 46; i++)
        {
            ON_CALL(leftEncoder, getTicks())
                .WillByDefault(Return(fakeGetTicks(leftMotor, &enslavement, maxVelocity)));
            ON_CALL(rightEncoder, getTicks())
                .WillByDefault(Return(fakeGetTicks(rightMotor, &enslavement, maxVelocity)));

            //Test is based on Serial output. Ctrl+c to end. TODO: find better solution
            enslavement.compute();
            //std::this_thread::sleep_for(std::chrono::milliseconds(deltaT));
        }
    }

    TEST(Enslavement, turn)
    {
        //Ridiculously high value because of very high acceleration
        //TODO: reduce acceleration given the objective
        enslavement.turn(21000);

        for (int i = 0; i < 46; i++)
        {
            ON_CALL(leftEncoder, getTicks())
                .WillByDefault(Return(fakeGetTicks(leftMotor, &enslavement, maxVelocity)));
            ON_CALL(rightEncoder, getTicks())
                .WillByDefault(Return(fakeGetTicks(rightMotor, &enslavement, maxVelocity)));

            //Test is based on Serial output. Ctrl+c to end. TODO: find better solution
            enslavement.compute();
            //std::this_thread::sleep_for(std::chrono::milliseconds(deltaT));
        }
    }
}
