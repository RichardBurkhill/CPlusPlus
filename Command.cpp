/**
 * @file command.cpp
 * @brief Demonstrates the Command pattern in C++.
 * @details Used to encapsulate a request as an object, thereby allowing users to parameterize clients with different requests.
 */
#include <iostream>

class Command {
public:
    virtual void execute() = 0;
    virtual ~Command() = default;
};

class Receiver {
public:
    void action() { std::cout << "Action executed\n"; }
};

class ConcreteCommand : public Command {
    Receiver& receiver_;
public:
    ConcreteCommand(Receiver& r) : receiver_(r) {}
    void execute() override { receiver_.action(); }
};
