/**
 * @file factory.cpp
 * @brief Demonstrates the Factory Method pattern in C++.
 * @details Used to create objects without specifying the exact class.
 */
#include <string>
#include <memory>

class Product {
public:
    virtual std::string name() const = 0;
    virtual ~Product() = default;
};

class ConcreteProduct : public Product {
public:
    std::string name() const override { return "ConcreteProduct"; }
};

class Creator {
public:
    virtual std::unique_ptr<Product> create() const = 0;
};

class ConcreteCreator : public Creator {
public:
    std::unique_ptr<Product> create() const override {
        return std::make_unique<ConcreteProduct>();
    }
};