#include "catch.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <string_view>

using namespace std;

template <typename T>
T maximum(T a, T b)
{
    //undeclared(); // ERROR - phase 1
    puts(__PRETTY_FUNCTION__);
    return a < b ? b : a; // maybe ERROR - phase 2
}

const char* maximum(const char* a, const char* b)
{
    puts(__PRETTY_FUNCTION__);
    return strcmp(a, b) < 0 ? b : a;
}

TEST_CASE("function template")
{
    // REQUIRE(maximum(1, 2) == 2);
    // REQUIRE(maximum(3.14, 6.33) == Approx(6.33));
    std::string text = "def";
    REQUIRE(maximum("abc"s, text) == "def"s);
    REQUIRE(maximum<std::string>("abc", text) == "def"s);

    std::vector<int> v1 {1, 2, 3};
    std::vector<int> v2 {4, 5, 6};
    REQUIRE(maximum(v1, v2) == v2);

    const char* ctxt1 = "abc";
    const char* ctxt2 = "def";

    REQUIRE(maximum(ctxt1, ctxt2) == "def");
}

namespace ver_1
{
    template <typename TResult, typename T1, typename T2>
    TResult add(const T1& a, const T2& b)
    {
        return a + b;
    }
}

namespace ver_2
{
    template <typename T1, typename T2>
    auto add(const T1& a, const T2& b) -> decltype(a + b)
    {
        return a + b;
    }
}

namespace ver_3
{
    template <typename T1, typename T2>
    std::common_type_t<T1, T2> maximum(const T1& a, const T2& b)
    {
        return a < b ? b : a;
    }
}

namespace ver_4
{
    template <typename T1, typename T2, typename ResultT = std::common_type_t<T1, T2>>
    ResultT maximum(const T1& a, const T2& b)
    {
        return a < b ? b : a;
    }
}

TEST_CASE("returned type")
{
    using ver_2::add;

    auto r1 = add(3, 3.14);

    static_assert(is_same<decltype(r1), double>::value, "Error");
    REQUIRE(r1 == Approx(6.14));

    uint8_t x1 = 56;
    uint64_t x2 = 343434;

    REQUIRE(ver_4::maximum(x1, x2) == 343434);
}

struct InPlaceT {};
constexpr static InPlaceT holder_in_place;

template <typename T>
class Holder
{
    T item_;
public:
    Holder(const T& item) : item_{item}
    {}

    Holder(T&& item) : item_{std::move(item)}
    {}

    template <typename... ItemT>
    Holder(InPlaceT, ItemT&&... item) : item_(std::forward<ItemT>(item)...)
    {
    }

    T& value()
    {
        return item_;
    }

    const T& value() const
    {
        return item_;
    }

    void info() const 
    {
        std::cout << "Holder<T: " << typeid(T).name() << ">(" << item_ << ")\n";
    }
};

template <typename T>
class Holder<T*>
{
    T* ptr_;
public:
    using value_type = T;

    explicit Holder(T* ptr) : ptr_{ptr}
    {}

    Holder(const Holder&) = delete;
    Holder& operator=(const Holder&) = delete;

    Holder(Holder&& other) noexcept : ptr_{other.ptr_}
    {
        other.ptr_ = nullptr;
    }

    Holder& operator=(Holder&& other) noexcept
    {
        if (this != &other)
        {
            this->~Holder();

            ptr_ = other.ptr_;
            other.ptr_ = nullptr;
        }

        return *this;
    }

    ~Holder() 
    {
        delete ptr_;
    }
    
    T& value() const
    {
        return *ptr_;
    }

    T* get() const
    {
        return ptr_;
    }

    void info() const 
    {
        std::cout << "Holder<T*: " << typeid(T).name() << ">(" << *ptr_ << ")\n";
    }
};

template <>
class Holder<const char*>
{
    const char* text_;
public:
    using value_type = const char*;

    Holder(const char* text) : text_{text}
    {}

    string_view value() const
    {
        return text_;
    }

    void info() const
    {
        cout << "Holder<const char*>(" << text_ << ")" << endl;
    }
};

TEST_CASE("Holder")
{
    string str = "abc";
    
    Holder<string> h1(str);
    Holder<string> h2(std::move(str));
    Holder<string> h3(holder_in_place, "text");
    Holder<string> h4(holder_in_place, 10, 'o');

    std::cout << h4.value() << " - ";
    h4.info();

    Holder<string*> h5(new string("dynamic"));
    h5.info();
    REQUIRE(h5.value() == "dynamic");

    Holder<const char*> h6("text");
    h6.info();
    REQUIRE(h6.value() == "text"sv);

    // since C++17 - Class Template Argument Deduction (CTAD)
    // Holder h7 = "text";
    // Holder h8 = 42;
    // Holder h9 = new string("dynamic text");
    std::vector vec = {1, 2, 3};
}


