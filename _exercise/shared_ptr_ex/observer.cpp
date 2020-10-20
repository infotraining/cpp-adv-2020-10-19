#include <cassert>
#include <cstdlib>
#include <iostream>
#include <set>
#include <stdexcept>
#include <string>

class Observer
{
public:
    virtual void update(const std::string& event_args) = 0;
    virtual ~Observer() { }
};

class Subject
{
    int state_;
    std::set<Observer*> observers_;

public:
    Subject()
        : state_(0)
    {
    }

    void register_observer(Observer* observer)
    {
        observers_.insert(observer);
    }

    void unregister_observer(Observer* observer)
    {
        observers_.erase(observer);
    }

    void set_state(int new_state)
    {
        if (state_ != new_state)
        {
            state_ = new_state;
            notify("State has been set to: " + std::to_string(state_));
        }
    }

protected:
    void notify(const std::string& event_args)
    {
        for (std::set<Observer*>::iterator it = observers_.begin(); it != observers_.end(); ++it)
        {
            (*it)->update(event_args);
        }
    }
};

class Customer : public Observer
{
public:
    void update(const std::string& event)
    {
        std::cout << "Customer notified: " << event << std::endl;
    }
};

class Logger : public Observer
{
    std::string path_;

public:
    Logger(const std::string& p)
        : path_{p}
    {
    }

    void update(const std::string& event)
    {
        std::cout << "Logging to " << path_ << ": " << event << std::endl;
    }

    ~Logger()
    {
        std::cout << "Logger " << path_ << " is closed...\n";
    }
};

int main(int argc, char const* argv[])
{
    using namespace std;

    Subject s;

    {
        Customer* o1 = new Customer();
        s.register_observer(o1);

        {
            Logger* logger = new Logger("log-20200707-12:22:33.dat");
            s.register_observer(logger);

            s.set_state(1);
            s.set_state(2);
            s.set_state(3);

            delete logger;
        }
        cout << "++++ End of scope ++++\n\n"
             << endl;

        s.set_state(42);
        s.set_state(665);
        s.unregister_observer(o1);
    }
}
