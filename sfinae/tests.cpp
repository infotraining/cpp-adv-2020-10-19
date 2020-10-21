#include "catch.hpp"
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace Catch::Matchers;

template <bool Condition, typename T = void>
struct EnableIf
{
    using type = T;
};

template <typename T>
struct EnableIf<false, T>
{
};

template <bool Condition, typename T>
using EnableIf_t = typename EnableIf<Condition, T>::type;

TEST_CASE("enable_if")
{
    static_assert(is_same<EnableIf<true>::type, void>::value, "Error");
    //static_assert(is_same<EnableIf<false>::type, void>::value, "Error");

    static_assert(is_same<EnableIf<std::is_integral<int>::value, bool>::type, bool>::value, "Error");
}

template <typename T>
auto is_power_of_2(T value) -> std::enable_if_t<std::is_integral<T>::value, bool>
{
    return value > 0 && (value & (value - 1)) == 0;
}

template <typename T>
auto is_power_of_2(T value) -> std::enable_if_t<std::is_floating_point<T>::value, bool>
{
    int exponent;
    const T mantissa = std::frexp(value, &exponent);
    return mantissa == T {0.5};
}

namespace AltTake
{
    template <typename T,
        typename = std::enable_if_t<std::is_integral<T>::value>>
    bool is_power_of_2(T value)
    {
        return value > 0 && (value & (value - 1)) == 0;
    }

    template <typename T, typename = void,
        typename = std::enable_if_t<std::is_floating_point<T>::value>>
    bool is_power_of_2(T value)
    {
        int exponent;
        const T mantissa = std::frexp(value, &exponent);
        return
    }
}

// namespace Cpp20
// {
//     bool is_power_of_2(integral auto value) 
//     {
//         return value > 0 && (value & (value - 1)) == 0;
//     }

//     bool is_power_of_2(floating_point auto T value)
//     {
//         int exponent;
//         const T mantissa = std::frexp(value, &exponent);
//         return mantissa == T {0.5};
//     }
// }

TEST_CASE("Test")
{
    REQUIRE(is_power_of_2(4));
    REQUIRE(is_power_of_2(32L));
    REQUIRE(!is_power_of_2(65));

    REQUIRE(is_power_of_2(8.0f));
}