#include "catch.hpp"
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace Catch::Matchers;

string full_name(const string& first, const string& last)
{
    return first + " " + last;
}

TEST_CASE("reference binding")
{
    std::string name = "jan";

    SECTION("C++98")
    {
        std::string& ref_name = name;

        const std::string& ref_full_name = full_name(name, "kowalski");
        REQUIRE(ref_full_name.size() == 12);
        //ref_full_name[0] = 'J';
    }

    SECTION("C++11")
    {
        std::string&& rv_ref_full_name = full_name(name, "kowalski");

        REQUIRE(rv_ref_full_name.size() == 12);
        rv_ref_full_name[0] = 'J';

        REQUIRE(rv_ref_full_name == "Jan kowalski");

        //std::string&& rv_ref_name = name; // ERROR - ill formed
    }
}

namespace Explain
{
    template <typename T>
    class vector
    {
    public:
        void push_back(const T& item)
        {
            std::cout << "push_back " << item << " by copy\n";
        }

        void push_back(T&& item)
        {
            std::cout << "push_back " << item << " by move\n";
        }
    };
}

TEST_CASE("vector + push_back")
{
    std::string name = "Jan";

    Explain::vector<std::string> vec;

    vec.push_back(name); // copy - const string&
    vec.push_back("Adam"); // move - string&&
    vec.push_back(name + "uary"); // move - string&&
    vec.push_back(std::move(name)); // move - string&&
}

class Gadget
{
public:
    int id_ {};
    std::string name_ {};

    Gadget() = default; // user declared

    Gadget(int id, const std::string& name)
        : id_ {id}
        , name_ {name}
    {
        std::cout << "Gadget(" << id_ << " " << name_ << ")\n";
    }

    Gadget(const Gadget&) = default;
    Gadget& operator=(const Gadget&) = default;
    Gadget(Gadget&&) = default;
    Gadget& operator=(Gadget&&) = default;

    ~Gadget() // user provided
    {
        std::cout << "~Gadget(" << id_ << " " << name_ << ")\n";
    }

    void use() const
    {
        std::cout << "Using Gadget(" << id_ << ", " << name_ << ")\n";
    }
};

TEST_CASE("default move semantics - rule of five")
{
    Gadget g {1, "ipa________________________________________d"};

    SECTION("copy")
    {
        std::cout << "making copy of g\n";

        Gadget copy_of_g = g; // copy
        REQUIRE((copy_of_g.id_ == 1 && copy_of_g.name_ == "ipa________________________________________d"s));
    }

    SECTION("move")
    {
        std::cout << "making move of g\n";
        
        Gadget target_g = std::move(g);
        REQUIRE((target_g.id_ == 1 && target_g.name_ == "ipa________________________________________d"s));       
        REQUIRE((g.id_ == 1 && g.name_ == ""s));       
    }
}

template <typename T>
class UniquePtr
{
    T* ptr_;
public:
    explicit UniquePtr(T* ptr) : ptr_{ptr}
    {}

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    // move constructor
    UniquePtr(UniquePtr&& source) : ptr_{source.ptr_}
    {
        // set to resourcless state
        source.ptr_ = nullptr;
    }

    // move assignment
    UniquePtr& operator=(UniquePtr&& source)
    {
        if (this != &source)
        {
            delete ptr_; // clean-up of previous state
            ptr_ = source.ptr_;
            source.ptr_ = nullptr;
        }

        return *this;
    }

    ~UniquePtr()
    {
        delete ptr_;
    }

    operator bool() const
    {
        return ptr_ != nullptr;
    }

    T& operator*() const
    {
        return *ptr_;
    }

    T* operator->() const
    {
        return ptr_;
    }

    T* get() const
    {
        return ptr_;
    }
};

TEST_CASE("custom move semantics")
{
    UniquePtr<Gadget> ptr_g1(new Gadget{32, "tablet"});

    ptr_g1->use();

    //UniquePtr<Gadget> ptr_g2 = ptr_g1; // ERROR

    UniquePtr<Gadget> ptr_g3 = std::move(ptr_g1); // ERROR
    
    REQUIRE(ptr_g1.get() == nullptr);
    ptr_g3->use();
} // tablet is destroyed

struct AllByDefault
{   
    int number;
    std::string text;
    std::vector<int> data;
};

TEST_CASE("all by default")
{
    AllByDefault a;

    a.number = 1;
    a.text = "text";
    a.data = {2, 3, 4};

    AllByDefault b = a;
    REQUIRE((b.number == 1 && b.text == "text"s && b.data == std::vector<int>{2, 3, 4}));   

    AllByDefault c = std::move(a);
    REQUIRE((c.number == 1 && c.text == "text"s && c.data == std::vector<int>{2, 3, 4})); 
    REQUIRE((a.text == ""s && a.data.size() == 0));
}