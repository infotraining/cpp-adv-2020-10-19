#include "catch.hpp"
#include <cstdio>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;
using namespace Catch::Matchers;

struct X
{
    int value;

    X(int v, const string& name)
        : value {v}
    {
        cout << "X(" << value << ", " << name << ")\n";
    }

    X()
    {
        static int gen_id = 0;
        value = ++gen_id;

        cout << "X(" << value << ")\n";
    }

    X(const X&) = delete;
    X(X&&) = delete;
    X& operator=(const X&) = delete;
    X& operator=(X&&) = delete;

    ~X()
    {
        cout << "~X(" << value << ")\n";
    }
};

namespace legacy_code
{
    X* get_x();

    X* get_x()
    {
        return new X(665, "less evil");
    }

    void use_x(X* ptr);

    void use_x(X* ptr)
    {
        if (ptr)
            cout << "using X: " << ptr->value << endl;

        delete ptr;
    }

    void use(X* ptr);

    void use(X* ptr)
    {
        if (ptr)
            cout << "using X: " << ptr->value << endl;
    }
}

TEST_CASE("legacy hell")
{
    using namespace legacy_code;

    {
        X* ptr = get_x();
        //...
    } // mem-leak

    {
        use(get_x());
    } // mem-leak

    {
        X x {1, "evil"};
        X* ptr = &x;
        use_x(ptr); // seg-fault
    }
}

namespace modernized
{
    std::unique_ptr<X> get_x();

    std::unique_ptr<X> get_x()
    {
        //return std::unique_ptr<X>(new X(665, "less evil"));
        return std::make_unique<X>(665, "less evil");
    }

    std::unique_ptr<X> use_x(std::unique_ptr<X> ptr);

    std::unique_ptr<X> use_x(std::unique_ptr<X> ptr)
    {
        if (ptr)
            cout << "using X: " << ptr->value << endl;

        return ptr;
    }

    void use(X* ptr);

    void use(X* ptr)
    {
        if (ptr)
            cout << "using X: " << ptr->value << endl;
    }
}

TEST_CASE("leak free code")
{
    using namespace modernized;

    {
        std::unique_ptr<X> ptr = get_x();
        REQUIRE(ptr->value == 665);
        //...

        auto ptr_other = std::move(ptr);
        REQUIRE(ptr_other->value == 665);

        std::vector<std::unique_ptr<X>> ptrs;
        ptrs.push_back(std::move(ptr_other));
        REQUIRE(ptrs[0]->value == 665);
    }

    {
        auto ptr_x = get_x();
        use(ptr_x.get());
        use(get_x().get());
    }

    {
        use_x(get_x());
        auto ptr_x = std::make_unique<X>(42, "xxx");
        auto target_x = use_x(std::move(ptr_x));
        REQUIRE(target_x->value == 42);
    }
}

TEST_CASE("custom deallocator")
{
    SECTION("unsafe")
    {
        FILE* f = fopen("data.txt", "w");

        vector<int> vec;
        fprintf(f, "%d", vec.at(5)); // throws exception

        fclose(f);
    }

    SECTION("safe")
    {
        std::unique_ptr<FILE, int (*)(FILE*)> f {fopen("data2.txt", "w"), fclose};

        vector<int> vec;
        fprintf(f.get(), "%d", vec.at(5)); // throws exception
    }

    SECTION("safe")
    {
        auto file_closer = [](FILE* f) { fclose(f); };

        std::unique_ptr<FILE, decltype(file_closer)> f {fopen("data2.txt", "w"), file_closer};

        vector<int> vec;
        fprintf(f.get(), "%d", vec.at(5)); // throws exception
    }
}

TEST_CASE("std::unique_ptr<T[]>")
{   
    int* tab = new int[100];

    std::unique_ptr<int[]> uptr(tab);

    uptr[54] = 
}