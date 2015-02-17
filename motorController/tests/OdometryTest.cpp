#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Odometry.h"
#include "./mocks/MockEncoder.h"

using ::testing::AtLeast;
using ::testing::Return;



TEST(Odometry, testSuite)
{
    MockEncoder leftEncoder(1, 2);
    MockEncoder rightEncoder(3, 4);

    ON_CALL(leftEncoder, getTicks()).WillByDefault(Return(10));

    std::cout << leftEncoder.getTicks();
}