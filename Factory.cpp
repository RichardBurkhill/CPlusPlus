/**
 * @file Factory.cpp
 * @brief Demonstrates the Factory Method pattern in C++.
 * @details The Factory Method design pattern is a creational design pattern that provides an interface
 *          for creating objects in a superclass, but allows subclasses to alter the type of objects that
 *          will be created. It defers object creation to subclasses.
 *
 *          Key components of the Factory Method pattern:
 *          - **Product (interface/abstract class):** Declares the interface for objects the factory method produces.
 *          - **ConcreteProduct:** Implements the Product interface.
 *          - **Creator (interface/abstract class):** Declares the factory method, which returns an object of type Product.
 *            It may also define a default implementation of the factory method that returns a default ConcreteProduct object.
 *          - **ConcreteCreator:** Overrides the factory method to return an instance of a ConcreteProduct.
 */
#include <string>   // Required for std::string
#include <memory>   // Required for std::unique_ptr and std::make_unique
#include <iostream> // Required for std::cout in example usage

/**
 * @class Product
 * @brief The abstract base class (interface) for objects created by the factory method.
 * @details Defines the common interface for all concrete products.
 */
class Product {
public:
    /**
     * @brief Pure virtual method to get the name of the product.
     * @details All concrete product classes must implement this method.
     * @return A string representing the name of the product.
     */
    virtual std::string name() const = 0;

    /**
     * @brief Virtual destructor to ensure proper cleanup of derived classes.
     * @details Declared as default to allow compiler-generated destructor,
     *          but ensures polymorphism works correctly when deleting Product pointers.
     */
    virtual ~Product() = default;
};

/**
 * @class ConcreteProduct
 * @brief Implements the Product interface.
 * @details This is a concrete implementation of the product that the factory will create.
 */
class ConcreteProduct : public Product {
public:
    /**
     * @brief Returns the name of this concrete product.
     * @return A string "ConcreteProduct".
     */
    std::string name() const override { return "ConcreteProduct"; } // Returns the specific name of this product
};

/**
 * @class Creator
 * @brief The abstract base class for creators.
 * @details Declares the factory method, which returns an object of type Product.
 *          It may also define a default implementation of the factory method.
 */
class Creator {
public:
    /**
     * @brief Pure virtual factory method to create a Product object.
     * @details Concrete creator classes will implement this method to return a specific
     *          ConcreteProduct. Uses std::unique_ptr for automatic memory management.
     * @return A unique pointer to a Product object.
     */
    virtual std::unique_ptr<Product> create() const = 0;

    /**
     * @brief Virtual destructor to ensure proper cleanup of derived classes.
     * @details Ensures polymorphism works correctly when deleting Creator pointers.
     */
    virtual ~Creator() = default;
};

/**
 * @class ConcreteCreator
 * @brief Implements the Creator interface, overriding the factory method.
 * @details This concrete creator produces ConcreteProduct objects.
 */
class ConcreteCreator : public Creator {
public:
    /**
     * @brief Overrides the factory method to create and return a ConcreteProduct.
     * @details This method is the core of the Factory Method pattern, deferring
     *          the instantiation of ConcreteProduct to this subclass.
     * @return A unique pointer to a ConcreteProduct object.
     */
    std::unique_ptr<Product> create() const override {
        return std::make_unique<ConcreteProduct>(); // Creates and returns a ConcreteProduct
    }
};

// Example of how to use the Factory Method pattern
/*
int main() {
    // Create a ConcreteCreator
    std::unique_ptr<Creator> creator = std::make_unique<ConcreteCreator>();

    // Use the factory method to create a product without knowing its concrete type
    std::unique_ptr<Product> product = creator->create();

    // Use the product
    std::cout << "Created product: " << product->name() << std::endl; // Output the product's name

    return 0;
}
*/
