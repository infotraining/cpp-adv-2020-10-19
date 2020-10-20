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
        : value_{value}
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
}

void reset_value(Gadget& g, int n)
{
    // some logic

    g.set_value(n * n);
    cout << "New value for Gadget: " << g.value() << endl;
}

Gadget* create_gadget(int arg)
{
    return new Gadget(arg);
}

void unsafe1() // TODO: poprawa z wykorzystaniem smart_ptr
{
    Gadget* ptr_gdgt = create_gadget(4);

    /* kod korzystajacy z ptrX */

    reset_value(*ptr_gdgt, 5);

    ptr_gdgt->unsafe();

    delete ptr_gdgt;
}

void unsafe2()
{
    int size = 10;

    Gadget* buffer = LegacyCode::create_many_gadgets(size);

    /* kod korzystający z buffer */

    for (int i = 0; i < size; ++i)
        buffer[0].unsafe();

    delete[] buffer;
}

void unsafe3()
{
    vector<Gadget*> my_gadgets;

    my_gadgets.push_back(create_gadget(87));
    my_gadgets.push_back(create_gadget(12));
    my_gadgets.push_back(new Gadget(98));

    int value_generator = 0;
    for (vector<Gadget*>::iterator it = my_gadgets.begin(); it != my_gadgets.end(); ++it)
    {
        cout << "Gadget's old value: " << (*it)->value() << endl;
        reset_value(**it, ++value_generator);
    }

    delete my_gadgets[0];
    my_gadgets[0] = 0;

    my_gadgets[1]->unsafe();

    // cleanup
    for (vector<Gadget*>::iterator it = my_gadgets.begin(); it != my_gadgets.end(); ++it)
        delete *it;
}

int main() try
{
    try
    {
        unsafe1();
        unsafe2();
        unsafe3();
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
