/**
 * @file facade.cpp
 * @brief Demonstrates the Facade pattern in C++.
 * @details Simplifies access to a complex subsystem.
 */
#include <iostream>

class SubsystemA {
public:
    void doA() const { std::cout << "A\n"; }
};
class SubsystemB {
public:
    void doB() const { std::cout << "B\n"; }
};

class Facade {
    SubsystemA a_;
    SubsystemB b_;
public:
    void operation() {
        a_.doA();
        b_.doB();
    }
};
