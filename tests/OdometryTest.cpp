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
 

namespace OdometryTest
{
    // NiceMock are used to avoid useless warnings
    testing::NiceMock<MockEncoder> leftEncoder(1, 2);
    testing::NiceMock<MockEncoder> rightEncoder(3, 4);
    Odometry *odometry = Odometry::getInst(&leftEncoder, &rightEncoder);
     
     
    TEST(Odometry, straightLine)
    {
        srand(time(NULL));

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
          .WillOnce(Return(Odometry::metersToTicks(M_PI/2)));
        EXPECT_CALL(rightEncoder, getTicks())    
          .WillOnce(Return(Odometry::metersToTicks(0)));
     
        odometry->update();
     
        CarthesianCoordinates coordinates = odometry->getCoordinates();
     
        ASSERT_EQ(roundf(coordinates.y * 100) / 100, -0.5);
        ASSERT_EQ(roundf(coordinates.x * 100) / 100, 2.5);
       
    }
     
    TEST(Odometry, valueTest3)
    {
        EXPECT_CALL(leftEncoder, getTicks())                  
          .WillOnce(Return(Odometry::metersToTicks(M_PI/2)));
        EXPECT_CALL(rightEncoder, getTicks())    
          .WillOnce(Return(Odometry::metersToTicks(M_PI)));
     
        odometry->update();
     
        CarthesianCoordinates coordinates = odometry->getCoordinates();
     
        ASSERT_EQ(roundf(coordinates.y * 100) / 100, -2);
        ASSERT_EQ(roundf(coordinates.x * 100) / 100, 4);
       
    }
     
    TEST(Odometry, valueTest4)
    {
        EXPECT_CALL(leftEncoder, getTicks())                  
          .WillOnce(Return(Odometry::metersToTicks(-M_PI/4)));
        EXPECT_CALL(rightEncoder, getTicks())    
          .WillOnce(Return(Odometry::metersToTicks(M_PI/4)));
     
        odometry->update();
     
        CarthesianCoordinates coordinates = odometry->getCoordinates();
        ASSERT_EQ(roundf(coordinates.y * 100) / 100, -2);
        ASSERT_EQ(roundf(coordinates.x * 100) / 100, 4);
       
    }
     
    TEST(Odometry, valueTest5)
    {
        EXPECT_CALL(leftEncoder, getTicks())                  
          .WillOnce(Return(Odometry::metersToTicks(3 * M_PI / 2)));
        EXPECT_CALL(rightEncoder, getTicks())    
          .WillOnce(Return(Odometry::metersToTicks(3 * M_PI)));
     
        odometry->update();
     
        CarthesianCoordinates coordinates = odometry->getCoordinates();
        ASSERT_EQ(roundf(coordinates.y * 100) / 100, -3.5);
        ASSERT_EQ(roundf(coordinates.x * 100) / 100, 2.5);
       
    }
}