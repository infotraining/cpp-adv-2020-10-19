#include <exception>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

using namespace std;

class Gadget
{
public:
    Gadget(int value = 0)
        : value_ {value}
    {
        std::cout << "Ctor Gadget(" << value_ << ")\n";
    }

    ~Gadget()
    {
        std::cout << "~Gadget(" << value_ << ")\n";
    }

    int value() const
    {
        return value_;
    }

    void set_value(int value)
    {
        value_ = value;
    }

    void unsafe()
    {
        throw std::runtime_error("ERROR");
    }

private:
    int value_;
};

namespace LegacyCode
{
    Gadget* create_many_gadgets(unsigned int size)
    {
        Gadget* xarray = new Gadget[size];

        for (unsigned int i = 0; i < size; ++i)
            xarray[i].set_value(i);

        return xarray;
    }

    void free_gadget(Gadget* ptr)
    {
        //..
        delete ptr;
    }
}

void reset_value(Gadget& g, int n)
{
    // some logic

    g.set_value(n * n);
    cout << "New value for Gadget: " << g.value() << endl;
}

std::unique_ptr<Gadget> create_gadget(int arg) 
{
    return std::unique_ptr<Gadget>(new Gadget(arg));
}

void safe1() 
{
    std::unique_ptr<Gadget> ptr_gdgt = create_gadget(4);

    /* kod korzystajacy z ptrX */

    reset_value(*ptr_gdgt, 5);

    ptr_gdgt->unsafe();
}

void safe2() // TODO: poprawa z wykorzystaniem smart_ptr
{
    int size = 10;

    std::unique_ptr<Gadget[]> buffer(LegacyCode::create_many_gadgets(size));

    /* kod korzystający z buffer */

    for (int i = 0; i < size; ++i)
        buffer[i].unsafe();
}

void safe3() 
{
    std::vector<std::unique_ptr<Gadget>> my_gadgets;

    my_gadgets.push_back(create_gadget(87));
    my_gadgets.push_back(create_gadget(12));
    my_gadgets.push_back(std::make_unique<Gadget>(98));

    int value_generator = 0;
    for (auto& ptr_g : my_gadgets)
    {
        cout << "Gadget's old value: " << ptr_g->value() << endl;
        reset_value(*ptr_g, ++value_generator);
    }

    //LegacyCode::free_gadget(my_gadgets[0].release());    
    my_gadgets[0].reset();

    my_gadgets[1]->unsafe();
}

int main()
try
{
    try
    {
        safe1();
        safe2();
        safe3();
    }
    catch (const exception& e)
    {
        cout << "Exception caught: " << e.what() << endl;
    }
}
catch (const exception& e)
{
    cout << "Exception caught: " << e.what() << endl;
}