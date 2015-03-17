#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "Odometry.h"
#include "Encoder.h"
#include "Motor.h"
#include "./mocks/MockEncoder.h"

using ::testing::AtLeast;
using ::testing::Return;
using ::testing::Invoke;


ACTION_P2(FakeGetTicks, pwm, deltaT)
{
    long ticks = pwm * 3 / 255 * Motor::REDUCTOR_RATIO * Encoder::TICK_PER_SPIN * 1000 / deltaT;
    int sign = rand() % 2 ? 1 : -1;
    float noise = (float)(rand() % 10) / 100;

    return ticks * (1 + sign * noise);
}


testing::NiceMock<MockEncoder> leftEncoder(1, 2);
testing::NiceMock<MockEncoder> rightEncoder(3, 4);
Odometry *odometry = Odometry::getInst(&leftEncoder, &rightEncoder);


TEST(Odometry, straightLine)
{
    srand(time(NULL));

    // NiceMock are used to avoid useless warnings


    // // Override getTicks
    // ON_CALL(leftEncoder, getTicks())
    //     .WillByDefault(FakeGetTicks(255, 15000));
    // ON_CALL(rightEncoder, getTicks())
    //     .WillByDefault(FakeGetTicks(255, 15000));


    EXPECT_CALL(leftEncoder, getTicks())
      .WillOnce(Return(Odometry::metersToTicks(2)));
    EXPECT_CALL(rightEncoder, getTicks())
      .WillOnce(Return(Odometry::metersToTicks(2)));

    odometry->update();
    CarthesianCoordinates coordinates = odometry->getCoordinates();

    ASSERT_EQ(roundf(coordinates.y * 100) / 100, 0);
    ASSERT_EQ(roundf(coordinates.x * 100) / 100, 2);
}


TEST(Odometry, valueTest2)
{
    EXPECT_CALL(leftEncoder, getTicks())
      .WillOnce(Return(Odometry::metersToTicks(-M_PI/2)));
    EXPECT_CALL(rightEncoder, getTicks())
      .WillOnce(Return(Odometry::metersToTicks(0)));

    odometry->update();

    CarthesianCoordinates coordinates = odometry->getCoordinates();

    ASSERT_EQ(roundf(coordinates.y * 100) / 100, -0.5);
    ASSERT_EQ(roundf(coordinates.x * 100) / 100, 1.5);

}

TEST(Odometry, valueTest3)
{
    EXPECT_CALL(leftEncoder, getTicks())
      .WillOnce(Return(Odometry::metersToTicks(-M_PI/2)));
    EXPECT_CALL(rightEncoder, getTicks())
      .WillOnce(Return(Odometry::metersToTicks(-M_PI)));

    odometry->update();

    CarthesianCoordinates coordinates = odometry->getCoordinates();

    ASSERT_EQ(roundf(coordinates.y * 100) / 100, -2);
    ASSERT_EQ(roundf(coordinates.x * 100) / 100, 0);

}

TEST(Odometry, valueTest4)
{
    EXPECT_CALL(leftEncoder, getTicks())
      .WillOnce(Return(Odometry::metersToTicks(M_PI/4)));
    EXPECT_CALL(rightEncoder, getTicks())
      .WillOnce(Return(Odometry::metersToTicks(-M_PI/4)));

    odometry->update();

    CarthesianCoordinates coordinates = odometry->getCoordinates();
    ASSERT_EQ(roundf(coordinates.y * 100) / 100, -2);
    ASSERT_EQ(roundf(coordinates.x * 100) / 100, 0);

}
