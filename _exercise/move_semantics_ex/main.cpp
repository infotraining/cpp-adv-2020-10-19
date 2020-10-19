#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "paragraph.hpp"
#include <iostream>

using namespace std::literals;

// TEST_CASE("Moving paragraph")
// {
//     LegacyCode::Paragraph p{"***"};

//     SECTION("move constructor")
//     {
//         LegacyCode::Paragraph mp = std::move(p);

//         REQUIRE(mp.get_paragraph() == "***"s);
//         REQUIRE(p.get_paragraph() == nullptr);
//     }

//     SECTION("move assignment")
//     {
//         LegacyCode::Paragraph other{"###"};
//         other = std::move(p);

//         REQUIRE(other.get_paragraph() == "***"s);
//         REQUIRE(p.get_paragraph() == nullptr);
//     }
// }

// TEST_CASE("Moving text shape")
// {
//     Text txt{10, 20, "text"};

//     SECTION("move constructor")
//     {
//         Text other = std::move(txt);

//         REQUIRE(other.text() == "text"s);
//         REQUIRE(txt.text() == ""s);
//     }

//     SECTION("move assignment")
//     {
//         Text other{665, 42, "abc"};
//         other = std::move(txt);

//         REQUIRE(other.text() == "text"s);
//         REQUIRE(txt.text() == ""s);
//     }
// }

LegacyCode::Paragraph create_paragraph()
{
    LegacyCode::Paragraph p {"abc"};
    return p;
}

class Header
{
    LegacyCode::Paragraph p_;
    std::string header_;

public:
    // Header(LegacyCode::Paragraph&& p, std::string&& h)
    //     : p_{std::move(p)}, header_{std::move(h)}
    // {
    // }

    // Header(const LegacyCode::Paragraph& p, const std::string& h)
    //     : p_{p}, header_{h}
    // {
    // }

    // Header(const LegacyCode::Paragraph& p, std::string&& h)
    //     : p_{p}, header_{std::move(h)}
    // {
    // }

    // Header(LegacyCode::Paragraph&& p, const std::string& h)
    //     : p_{std::move(p)}, header_{h}
    // {
    // }

    // // good compromise
    // Header(LegacyCode::Paragraph p, std::string h)
    //     : p_ {std::move(p)}
    //     , header_ {std::move(h)}
    // {
    // }

    // the best solution
    template <typename P, typename H>
    Header(P&& p, H&& h0)
        : p_{std::forward<P>(p)}, header_{std::forward<H>(h)}
    {}
};

TEST_CASE("Paragraph")
{
    using namespace LegacyCode;

    Paragraph p = create_paragraph();
    p.render_at(10, 20);

    std::string header_str = "<h2>";

    std::cout << "\n\n";

    Header h1(create_paragraph(), "<h1>"); // init with rvalue
    Header h2 {p, header_str}; // init with lvalue (copy)
    REQUIRE(header_str == "<h2>");
    Header h3 {create_paragraph(), std::move(header_str)};
    
    header_str = "<h4>"; // re-assignment is only valid operation
}