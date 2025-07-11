#pragma once

#include <string>
#include <iostream>

// Base class
/**
 * @brief Base class for animals.
 * @details Demonstrates virtual functions and a pure virtual `speak` method,
 *          making `Animal` an abstract class.
 */
class Animal {
public:
    std::string _name;
    int _age;

    // Default constructor
    Animal() = default;

    // Custom constructor
    Animal(const std::string& name, int age) : _name(std::move(name)), _age(age) {}

    // Copy constructor
    Animal(const Animal& rhs) = default;

    // Copy assignment operator
    Animal& operator=(const Animal& other) = default;

    // Move constructor (optional but good)
    Animal(Animal&& other) noexcept = default;

    // Move assignment operator
    Animal& operator=(Animal&& other) noexcept = default;

    // Destructor
    ~Animal() = default;

    std::string getName() const {
        return _name;
    }

    void speak() const {
        std::cout << _name << " says hello, age " << _age << '\n';
    }
};

// C++11: Strongly typed enumerations
/**
 * @brief Enumeration for different animal types.
 * @details This is a C++11 feature, demonstrating strongly typed enumerations
 *          to prevent implicit conversions and name clashes.
 */
enum class AnimalType { Dog, Cat };

// C++14: constexpr function
/**
 * @brief Doubles an integer value at compile time.
 * @param age The integer value to double.
 * @return The doubled value.
 * @details This function demonstrates C++14's relaxed `constexpr` rules,
 *          allowing more complex logic within `constexpr` functions.
 */
constexpr int doubleAge(int age) { ///< C++14: Relaxed constexpr function, allowing more complex logic.
    return age * 2;
}