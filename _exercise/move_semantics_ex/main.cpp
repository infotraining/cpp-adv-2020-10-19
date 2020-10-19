#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "paragraph.hpp"
#include <iostream>

using namespace std::literals;

TEST_CASE("Moving paragraph")
{    
    LegacyCode::Paragraph p{"***"};

    SECTION("move constructor")
    {
        LegacyCode::Paragraph mp = std::move(p);

        REQUIRE(mp.get_paragraph() == "***"s);
        REQUIRE(p.get_paragraph() == nullptr);
    }

    SECTION("move assignment")
    {
        LegacyCode::Paragraph other{"###"};
        other = std::move(p);

        REQUIRE(other.get_paragraph() == "***"s);
        REQUIRE(p.get_paragraph() == nullptr);
    }
}

TEST_CASE("Moving text shape")
{
    Text txt{10, 20, "text"};

    SECTION("move constructor")
    {
        Text other = std::move(txt);

        REQUIRE(other.text() == "text"s);
        REQUIRE(txt.text() == ""s);
    }

    SECTION("move assignment")
    {
        Text other{665, 42, "abc"};
        other = std::move(txt);

        REQUIRE(other.text() == "text"s);
        REQUIRE(txt.text() == ""s);
    }
}