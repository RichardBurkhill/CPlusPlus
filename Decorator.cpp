/**
 * @file decorator.cpp
 * @brief Demonstrates the Decorator pattern in C++.
 * @details Used to add new behavior to objects dynamically.
 */
#include <iostream>
#include <memory>

class Component {
public:
    virtual std::string operation() const = 0;
    virtual ~Component() = default;
};

class ConcreteComponent : public Component {
public:
    std::string operation() const override { return "Component"; }
};

class Decorator : public Component {
    std::unique_ptr<Component> component_;
public:
    Decorator(std::unique_ptr<Component> comp) : component_(std::move(comp)) {}
    std::string operation() const override {
        return "Decorated(" + component_->operation() + ")";
    }
};
