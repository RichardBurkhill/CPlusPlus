/**
 * @file adapter.cpp
 * @brief Demonstrates the Adapter pattern in C++.
 * @details Used to convert one interface to another expected by clients.
 */
#include <iostream>

class OldInterface {
public:
    void specificRequest() const { std::cout << "Old interface\n"; }
};

class NewInterface {
public:
    virtual void request() const = 0;
};

class Adapter : public NewInterface {
    OldInterface old_;
public:
    void request() const override { old_.specificRequest(); }
};
