/**
 * @file Builder.cpp
 * @brief Demonstrates the Builder design pattern in C++.
 * @details The Builder pattern is used to construct a complex object step by step.
 *          It separates the construction of a complex object from its representation,
 *          allowing the same construction process to create different representations.
 */

#include <string>
#include <vector>
#include <iostream>

/**
 * @class Product
 * @brief Represents the complex object being built.
 * @details This class holds the various parts that make up the final product.
 */
class Product
{
private:
    std::vector<std::string> parts_; ///< @brief Stores the individual parts of the product.

public:
    /**
     * @brief Adds a part to the product.
     * @param part A string representing the part to be added.
     */
    void add(const std::string &part)
    {
        parts_.push_back(part); ///< Appends the new part to the internal vector.
    }

    /**
     * @brief Displays all parts of the product.
     * @details Iterates through the stored parts and prints them to the console.
     */
    void show() const
    {
        std::cout << "Product Parts: ";
        for (const auto &part : parts_)
        {
            std::cout << part << " "; ///< Prints each part followed by a space.
        }
        std::cout << std::endl; ///< Adds a newline at the end.
    }
};

/**
 * @class Builder
 * @brief Abstract base class for creating parts of a Product object.
 * @details Defines the interface for creating parts of a Product.
 *          Concrete builders will implement these methods to construct specific types of products.
 */
class Builder
{
protected:
    Product product_; ///< @brief The product object currently under construction.

public:
    /**
     * @brief Resets the builder to construct a new product.
     * @details This method should be called before starting a new product construction.
     */
    void reset()
    {
        product_ = Product(); ///< Initializes a new, empty Product object.
    }

    /**
     * @brief Adds Part A to the product.
     * @return A reference to the Builder object for method chaining.
     */
    Builder &addPartA()
    {
        product_.add("PartA"); ///< Adds "PartA" to the product.
        return *this;          ///< Enables fluent interface (method chaining).
    }

    /**
     * @brief Adds Part B to the product.
     * @return A reference to the Builder object for method chaining.
     */
    Builder &addPartB()
    {
        product_.add("PartB"); ///< Adds "PartB" to the product.
        return *this;          ///< Enables fluent interface (method chaining).
    }

    /**
     * @brief Retrieves the constructed Product object.
     * @return The fully built Product object.
     * @details This method finalizes the construction and returns the result.
     */
    Product build()
    {
        return product_; ///< Returns the constructed product.
    }
};

/**
 * @brief Main function to demonstrate the Builder pattern.
 * @details Creates a Builder object and uses it to construct a Product step-by-step,
 *          then displays the constructed product.
 */
int main()
{
    Builder builder; ///< @brief Create a Builder instance.

    // Construct a product with PartA and PartB
    std::cout << "Building Product 1:\n";
    builder.reset();                    ///< @brief Reset the builder for a new product.
    builder.addPartA().addPartB();      ///< @brief Chain calls to add parts.
    Product product1 = builder.build(); ///< @brief Get the constructed product.
    product1.show();                    ///< @brief Display the product parts.

    // Construct another product with only PartA
    std::cout << "\nBuilding Product 2:\n";
    builder.reset();                    ///< @brief Reset the builder for another new product.
    builder.addPartA();                 ///< @brief Add only PartA.
    Product product2 = builder.build(); ///< @brief Get the second constructed product.
    product2.show();                    ///< @brief Display the second product parts.

    return 0;
}
