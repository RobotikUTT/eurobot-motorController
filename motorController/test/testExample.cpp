#include "catch.hpp"

TEST_CASE("Test serie example", "test") {
    REQUIRE(1 == 1); //will work
    REQUIRE(2 == 1); //error !
}