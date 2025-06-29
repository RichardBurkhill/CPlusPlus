/**
 * @file strategy.cpp
 * @brief Demonstrates the Strategy pattern in C++.
 * @details Used to define a family of algorithms, encapsulate each one, and make them interchangeable.
 */
#include <iostream>
#include <memory>

class Strategy {
public:
    virtual void execute() const = 0;
    virtual ~Strategy() = default;
};

class ConcreteStrategyA : public Strategy {
public:
    void execute() const override { std::cout << "A\n"; }
};

class Context {
    std::unique_ptr<Strategy> strategy_;
public:
    Context(std::unique_ptr<Strategy> s) : strategy_(std::move(s)) {}
    void run() const { strategy_->execute(); }
};
