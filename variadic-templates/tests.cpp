#include "catch.hpp"
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace Catch::Matchers;

void print()
{
    std::cout << "\n";
}

template <typename Head, typename... Tail>
void print(Head head, Tail... tail)
{
    std::cout << head << " ";
    print(tail...);
}

namespace Cpp17
{
    template <typename Head, typename... Tail>
    void print(Head head, Tail... tail)
    {
        std::cout << head << " ";

        if constexpr (sizeof...(tail) > 0)
            print(tail...);
    }
}

TEST_CASE("head-tail")
{
    print(1, 3.14, "text"s, "abc");
    Cpp17::print(1, 3.14);
}