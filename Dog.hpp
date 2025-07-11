#include "Animal.hpp"

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
    Dog(const std::string& name, int age) : Animal(name, age) {}
    Dog() = default;

    Dog(const Dog& rhs) = default;
    Dog& operator=(const Dog& rhs) = default;
    Dog(Dog&& rhs) noexcept = default;
    Dog& operator=(Dog&& rhs) noexcept = default;
    ~Dog() = default;

    /**
     * @brief Makes the dog speak.
     * @details Overrides the base class `speak` method.
     */
    void speak() const {
        std::cout << _name << " says: Woof!" << std::endl;
    }
};
