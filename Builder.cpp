/**
 * @file builder.cpp
 * @brief Demonstrates the Builder design pattern in C++.
 * @details Used to construct complex objects step-by-step.
 */
#include <string>
#include <vector>
#include <iostream>

class Product {
public:
    void add(const std::string& part) { parts_.push_back(part); }
    void show() const {
        for (const auto& part : parts_) std::cout << part << " ";
    }
private:
    std::vector<std::string> parts_;
};

class Builder {
public:
    Builder& addPartA() { product_.add("PartA"); return *this; }
    Builder& addPartB() { product_.add("PartB"); return *this; }
    Product build() { return product_; }
private:
    Product product_;
};
