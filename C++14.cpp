/**
 * @file C++14.cpp
 * @brief Demonstrates various C++14 language constructs and features.
 *
 * This file showcases key C++14 additions including:
 * - `make_unique` for creating unique pointers.
 * - `constexpr` functions with more relaxed rules.
 * - Generic lambdas.
 * - Binary literals and digit separators.
 * - Return type deduction for functions.
 */

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <algorithm>

// C++11: Strongly typed enumerations
/**
 * @brief Enumeration for different animal types.
 * @details This is a C++11 feature, demonstrating strongly typed enumerations
 *          to prevent implicit conversions and name clashes.
 */
enum class AnimalType { Dog, Cat };

// Base class
/**
 * @brief Base class for animals.
 * @details Demonstrates virtual functions and a pure virtual `speak` method,
 *          making `Animal` an abstract class.
 */
class Animal {
public:
    /**
     * @brief Constructor for the Animal class.
     * @param name The name of the animal.
     */
    Animal(std::string name) : name_(std::move(name)) {}
    
    /**
     * @brief Virtual destructor for proper cleanup of derived classes.
     * @details Uses `default` keyword (C++11) to explicitly default the destructor.
     */
    virtual ~Animal() = default;

    // C++11: override keyword
    /**
     * @brief Pure virtual function for an animal to speak.
     * @details Marked with `override` (C++11) to ensure it overrides a base class method.
     */
    virtual void speak() const = 0;

    /**
     * @brief Gets the name of the animal.
     * @return The name of the animal.
     */
    std::string getName() const { return name_; }

protected:
    std::string name_;
};

// Dog class
/**
 * @brief Derived class representing a Dog.
 * @details Uses `final` (C++11) to prevent further derivation.
 */
class Dog final : public Animal {
public:
    /**
     * @brief Constructor for the Dog class.
     * @param name The name of the dog.
     */
    Dog(std::string name) : Animal(std::move(name)) {}

    /**
     * @brief Makes the dog speak.
     * @details Overrides the base class `speak` method.
     */
    void speak() const override {
        std::cout << name_ << " says: Woof!" << std::endl;
    }
};

// Cat class
/**
 * @brief Derived class representing a Cat.
 * @details Uses `final` (C++11) to prevent further derivation.
 */
class Cat final : public Animal {
public:
    /**
     * @brief Constructor for the Cat class.
     * @param name The name of the cat.
     */
    Cat(std::string name) : Animal(std::move(name)) {}

    /**
     * @brief Makes the cat speak.
     * @details Overrides the base class `speak` method.
     */
    void speak() const override {
        std::cout << name_ << " says: Meow!" << std::endl;
    }
};

// Zoo class manages animals using smart pointers
/**
 * @brief Manages a collection of animals using smart pointers.
 * @details Demonstrates C++11 features like deleted functions for copy control
 *          and default move constructors/assignment operators.
 */
class Zoo {
public:
    // C++11: deleted function to avoid copying
    /**
     * @brief Deleted copy constructor.
     * @details Prevents copying of `Zoo` objects (C++11).
     */
    Zoo(const Zoo&) = delete;
    
    /**
     * @brief Deleted copy assignment operator.
     * @details Prevents copying of `Zoo` objects (C++11).
     */
    Zoo& operator=(const Zoo&) = delete;

    /**
     * @brief Default constructor for Zoo.
     */
    Zoo() = default;

    // C++11: default move constructor
    /**
     * @brief Default move constructor.
     * @details Enables efficient moving of `Zoo` objects (C++11).
     */
    Zoo(Zoo&&) = default;
    
    /**
     * @brief Default move assignment operator.
     * @details Enables efficient moving of `Zoo` objects (C++11).
     */
    Zoo& operator=(Zoo&&) = default;

    /**
     * @brief Adds an animal to the zoo.
     * @param animal A unique pointer to the animal to add.
     */
    void addAnimal(std::unique_ptr<Animal> animal) {
        animals_.emplace_back(std::move(animal));
    }

    /**
     * @brief Makes all animals in the zoo speak.
     * @details Uses a C++11 range-based for loop for iteration.
     */
    void makeAllSpeak() const {
        // C++11: range-based for loop
        for (const auto& animal : animals_) { ///< C++11: Range-based for loop for convenient iteration.
            animal->speak();
        }
    }

    /**
     * @brief Lists the names of all animals in the zoo.
     * @details Uses a C++11 lambda expression with `std::for_each`.
     */
    void listAnimalNames() const {
        std::cout << "Animals in the zoo: ";
        // C++11: lambda
        std::for_each(animals_.begin(), animals_.end(),
                      [](const std::unique_ptr<Animal>& a) { ///< C++11: Lambda expression for concise inline function objects.
                          std::cout << a->getName() << " ";
                      });
        std::cout << std::endl;
    }

private:
    std::vector<std::unique_ptr<Animal>> animals_;
};

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

/**
 * @brief Main function demonstrating C++14 features.
 *
 * This function initializes a `Zoo` object, adds animals using `make_unique`,
 * and demonstrates a `constexpr` function.
 */
int main() {
    Zoo zoo;

    // C++14: make_unique
    zoo.addAnimal(std::make_unique<Dog>("Rex")); ///< C++14: std::make_unique for safe and efficient unique_ptr creation.
    zoo.addAnimal(std::make_unique<Cat>("Whiskers"));

    zoo.listAnimalNames();
    zoo.makeAllSpeak();

    constexpr int age = 4;
    std::cout << "Double age of " << age << " is " << doubleAge(age) << std::endl;

    return 0;
}
