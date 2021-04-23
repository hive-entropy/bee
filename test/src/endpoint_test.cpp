#include "catch.hpp"

SCENARIO("a message is received", "[endpoint]") {
    GIVEN("A matrix of int defined using an array") {
        WHEN("We do nothing") {
            THEN("Its elements should be accessible and be as defined as in the array") {
                REQUIRE(0 == 1);
            }
        }
    }
}