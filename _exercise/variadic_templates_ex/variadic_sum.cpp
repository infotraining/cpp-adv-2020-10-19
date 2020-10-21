#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include <cassert>
#include <iostream>
#include <type_traits>

using namespace std;

namespace vt
{
    // TODO
}

TEST_CASE("variadic sum")
{
    SECTION("for ints")
    {
        auto sum = vt::sum(1, 3, 3);

        REQUIRE(sum == 7);
        static_assert(is_same<int, decltype(sum)>::value, "Error");
    }

    SECTION("for floating points")
    {
        auto dbl_sum = vt::sum(1.1, 3.0f, 3);

        REQUIRE(dbl_sum == Approx(7.1));
        static_assert(is_same<double, decltype(dbl_sum)>::value, "Error");
    }

    SECTION("for strings")
    {
        auto text = vt::sum("Hello", string("world"), "!");

        REQUIRE(text == "Helloworld!");
        static_assert(is_same<string, decltype(text)>::value, "Error");
    }
}