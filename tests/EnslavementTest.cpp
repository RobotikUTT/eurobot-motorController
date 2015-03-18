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
    ACTION_P4(FakeGetTicks, motor, Vmax, deltaT, enslavement)
    {
        double ticks = Odometry::metersToTicks( (float)(motor->getPWM()) / (float) 255 * (float)Vmax );
        return ticks;
    }


    Motor *leftMotor = new Motor(0, 0, 0);
    Motor *rightMotor = new Motor(0, 0, 0);

    testing::NiceMock<MockEncoder> leftEncoder(1, 2);
    testing::NiceMock<MockEncoder> rightEncoder(3, 4);

    Odometry *odometry = Odometry::getInst(NULL, NULL);
    Enslavement enslavement(1000, 0.05, 1, leftMotor, rightMotor);


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
                .WillByDefault(FakeGetTicks(leftMotor, 1, 1000, enslavement));
            ON_CALL(rightEncoder, getTicks())
                .WillByDefault(FakeGetTicks(leftMotor, 1, 1000, enslavement));

            enslavement.compute();
            // std::this_thread::sleep_for(std::chrono::milliseconds(15));
        }
    }
}
