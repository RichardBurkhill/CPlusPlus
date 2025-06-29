/**
 * @file Prototype.cpp
 * @brief Demonstrates the Prototype design pattern in C++.
 * @details The Prototype pattern is a creational design pattern that allows new objects to be created by copying
 *          an existing object, known as the "prototype". This pattern is useful when the cost of creating an object
 *          is expensive or complex, or when you want to avoid coupling the client code to the concrete classes of objects.
 */
#include <memory> // Required for std::unique_ptr for managing object ownership

/**
 * @class Prototype
 * @brief The abstract base class for all prototype objects.
 * @details Defines the interface for cloning objects. Concrete prototypes must implement the clone method
 *          to return a copy of themselves.
 */
class Prototype {
public:
    /**
     * @brief Pure virtual method to be implemented by concrete prototypes.
     * @details This method is responsible for creating a new object that is a copy of the current object.
     * @return A unique pointer to a new Prototype object, which is a clone of the current object.
     */
    virtual std::unique_ptr<Prototype> clone() const = 0;

    /**
     * @brief Virtual destructor to ensure proper cleanup of derived prototype classes.
     * @details A virtual destructor is crucial when dealing with polymorphic classes to ensure that the correct
     *          destructor is called when an object is deleted via a base class pointer.
     */
    virtual ~Prototype() = default;
};

/**
 * @class ConcretePrototype
 * @brief A concrete implementation of the Prototype interface.
 * @details This class implements the clone method to create a deep copy of itself.
 *          It demonstrates how a specific object can be duplicated without knowing its concrete class.
 */
class ConcretePrototype : public Prototype {
public:
    /**
     * @brief Implements the clone method from the Prototype interface.
     * @details Creates a new `ConcretePrototype` object by copying the current object using the copy constructor.
     *          This ensures that all members of the original object are copied to the new object.
     * @return A unique pointer to a new `ConcretePrototype` object, which is a clone of the current object.
     */
    std::unique_ptr<Prototype> clone() const override {
        return std::make_unique<ConcretePrototype>(*this); // Use copy constructor to create a new object
    }
};
