#include "catch.hpp"
#include <array>
#include <iostream>
#include <map>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

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

    REQUIRE(maximum(ctxt1, ctxt2) == "def"s);
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

struct InPlaceT
{
};
constexpr static InPlaceT holder_in_place;

template <typename T>
class Holder
{
    T item_;

public:
    Holder(const T& item)
        : item_ {item}
    {
    }

    Holder(T&& item)
        : item_ {std::move(item)}
    {
    }

    template <typename... ItemT>
    Holder(InPlaceT, ItemT&&... item)
        : item_(std::forward<ItemT>(item)...)
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

// deduction guide
template <typename T>
Holder(T) -> Holder<T>;

template <typename T>
class Holder<T*>
{
    T* ptr_;

public:
    using value_type = T;

    explicit Holder(T* ptr)
        : ptr_ {ptr}
    {
    }

    Holder(const Holder&) = delete;
    Holder& operator=(const Holder&) = delete;

    Holder(Holder&& other) noexcept
        : ptr_ {other.ptr_}
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

    Holder(const char* text)
        : text_ {text}
    {
    }

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
    Holder h7 = "text";
    Holder h8 = 42;
    Holder h9(new string("dynamic text"));
    std::vector vec = {1, 2, 3};
}

namespace Tools
{
    template <typename T>
    size_t size(const T& container)
    {
        return container.size();
    }

    template <typename T, size_t N>
    size_t size(T (&arr)[N])
    {
        return N;
    }
}

TEST_CASE("arrays + size")
{
    int tab1[10];

    REQUIRE(Tools::size(tab1) == 10);

    int tab2[] = {1, 2, 3};

    REQUIRE(Tools::size(tab2) == 3);

    std::vector<int> vec {4, 5, 6, 7};

    REQUIRE(Tools::size(vec) == 4);
}

template <typename T, size_t N>
struct Array
{
    T items[N];

    using iterator = T*;
    using const_iterator = const T*;

    constexpr size_t size() const
    {
        return N;
    }

    iterator begin()
    {
        return &items[0];
    }

    iterator end()
    {
        return &items[0] + N;
    }

    const_iterator begin() const
    {
        return &items[0];
    }

    const_iterator end() const
    {
        return &items[0] + N;
    }

    T& operator[](size_t index)
    {
        return items[index];
    }

    const T& operator[](size_t index) const
    {
        return items[index];
    }
};

TEST_CASE("Array")
{
    Array<int, 10> arr = {};
    arr[4] = 5;
}

template <typename T, template <typename, size_t> class Container, size_t N = 1024>
class Stack
{
    Container<T, N> items_;
    size_t size_ {};

public:
    //...
    void push(const T& item)
    {
        items_[size_] = item;
        ++size_;
    }

    size_t size() const
    {
        return N;
    }
};

TEST_CASE("Stack")
{
    Stack<int, std::array> s1;
    Stack<int, Array, 2048> s2;
}

template <typename Container>
typename Container::value_type sum(const Container& container)
{
    using ResultT = typename Container::value_type;
    ResultT result {};

    for (typename Container::const_iterator it = container.begin(); it != container.end(); ++it)
    {
        result += *it;
    }

    return result;
}

//////////////////////////
// custom trait ElementT

template <typename T>
struct ElementT
{
    using type = typename T::value_type;
};

template <typename T, size_t N>
struct ElementT<T[N]>
{
    using type = T;
};

/////////////////////////////
// custom RemoveReference

template<typename T>
struct RemoveReference
{
    using type = T;
};

template <typename T>
struct RemoveReference<T&>
{
    using type = T;
};

template <typename T>
struct RemoveReference<T&&>
{
    using type = T;
};

template <typename T>
using RemoveReference_t = typename RemoveReference<T>::type;

TEST_CASE("remove_reference")
{
    static_assert(std::is_same<RemoveReference<int>::type, int>::value, "Error");
    static_assert(std::is_same<RemoveReference<int&>::type, int>::value, "Error");
    static_assert(std::is_same<RemoveReference<const int&>::type, const int>::value, "Error");

    static_assert(std::is_same<std::remove_const_t<int>, int>::value, "Error");
    static_assert(std::is_same<std::remove_const_t<const int>, int>::value, "Error");
}

namespace Cpp14
{
    template <typename Container>
    auto sum(const Container& container)
    {
        //using ResultT = typename ElementT<Container>::type; // typedef
        using ResultT = std::remove_cv_t<std::remove_reference_t<decltype(*begin(container))>>;

        ResultT result {};

        for (auto it = begin(container); it != end(container); ++it)
        {
            result += *it;
        }

        return result;
    }
}

TEST_CASE("sum")
{
    std::cout << "\n***************************\n";
    const std::vector<int> vec = {1, 2, 3};
    REQUIRE(Cpp14::sum(vec) == 6);

    const int tab[6] = {1, 1, 1, 1, 1, 1};
    REQUIRE(Cpp14::sum(tab));
}

template <typename T>
using StrKeyMap = std::map<std::string, T>;

template <typename... Ts>
using CommonType_t = typename std::common_type<Ts...>::type;

TEST_CASE("alias template")
{
    StrKeyMap<int> dict_en = {{"one", 1}, {"two", 2}};

    CommonType_t<int, double> pi = 3.14;
}

template <typename T>
constexpr T pi {3.1415926535897932385};

namespace BeforeCpp14
{
    template <typename T>
    struct Pi
    {
        static T value;
    };

    template <typename T>
    T Pi<T>::value {3.1415926535897932385};
}

TEST_CASE("variable templates")
{
    REQUIRE((BeforeCpp14::Pi<double>::value == Approx(3.1415926535897932385)));
    REQUIRE((BeforeCpp14::Pi<float>::value == Approx(3.1415926535897932385)));

    REQUIRE((pi<double> == Approx(3.1415926535897932385)));
    REQUIRE((pi<float> == Approx(3.1415926535897932385)));
}

template <typename T>
struct IsVoid : std::false_type
{};

template <>
struct IsVoid<void> : std::true_type
{};

template <typename T>
constexpr bool IsVoid_v = IsVoid<T>::value;


template <typename T>
auto get_value(T obj)
{
    static_assert(std::is_pointer<T>::value, "T must be pointer type");
    return *obj;
}

TEST_CASE("type traits")
{
    static_assert(std::true_type::value == true);
    static_assert(std::false_type::value == false);

    static_assert(!IsVoid<int>::value, "Error");
    static_assert(IsVoid_v<void>, "Error");

    int x = 10;
    int expected = get_value(&x);
    REQUIRE(expected == 10);
}

