#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "paragraph.hpp"
#include <iostream>

using namespace std::literals;

// TEST_CASE("Moving paragraph")
// {
//     Modernized::Paragraph p{"***"};

//     SECTION("move constructor")
//     {
//         Modernized::Paragraph mp = std::move(p);

//         REQUIRE(mp.get_paragraph() == "***"s);
//         REQUIRE(p.get_paragraph() == nullptr);
//     }

//     SECTION("move assignment")
//     {
//         Modernized::Paragraph other{"###"};
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

Modernized::Paragraph create_paragraph()
{
    Modernized::Paragraph p {"abc"};
    return p;
}

class Header
{
    Modernized::Paragraph p_;
    std::string header_;

public:
    // Header(Modernized::Paragraph&& p, std::string&& h)
    //     : p_{std::move(p)}, header_{std::move(h)}
    // {
    // }

    // Header(const Modernized::Paragraph& p, const std::string& h)
    //     : p_{p}, header_{h}
    // {
    // }

    // Header(const Modernized::Paragraph& p, std::string&& h)
    //     : p_{p}, header_{std::move(h)}
    // {
    // }

    // Header(Modernized::Paragraph&& p, const std::string& h)
    //     : p_{std::move(p)}, header_{h}
    // {
    // }

    // // good compromise
    // Header(Modernized::Paragraph p, std::string h)
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

// TEST_CASE("Paragraph")
// {
//     using namespace Modernized;

//     Paragraph p = create_paragraph();
//     p.render_at(10, 20);

//     std::string header_str = "<h2>";

//     std::cout << "\n\n";

//     Header h1(create_paragraph(), "<h1>"); // init with rvalue
//     Header h2 {p, header_str}; // init with lvalue (copy)
//     REQUIRE(header_str == "<h2>");
//     Header h3 {create_paragraph(), std::move(header_str)};
    
//     header_str = "<h4>"; // re-assignment is only valid operation
// }

TEST_CASE("vector + move semantics")
{
    using namespace Modernized;
    std::vector<Paragraph> vec;

    vec.push_back(Paragraph{"a"});
    std::cout << "\n";

    vec.push_back(Paragraph{"b"});
    std::cout << "\n";

    vec.push_back(Paragraph{"c"});
    std::cout << "\n";

    vec.push_back(Paragraph{"d"});
    std::cout << "\n";
}

TEST_CASE("move_if_noexcept")
{
    Modernized::Paragraph p{"abc"};

    auto target_p = std::move_if_noexcept(p);

    REQUIRE(target_p.get_paragraph() == "abc"s);
    REQUIRE(p.get_paragraph() == nullptr);
}