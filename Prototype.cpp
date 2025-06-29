/**
 * @file prototype.cpp
 * @brief Demonstrates the Prototype pattern in C++.
 * @details Used when new objects are to be created by copying existing ones.
 */
#include <memory>

class Prototype {
public:
    virtual std::unique_ptr<Prototype> clone() const = 0;
    virtual ~Prototype() = default;
};

class ConcretePrototype : public Prototype {
public:
    std::unique_ptr<Prototype> clone() const override {
        return std::make_unique<ConcretePrototype>(*this);
    }
};
