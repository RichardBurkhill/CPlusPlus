#include "Animal.hpp"

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
    Cat(const std::string& name, int age) : Animal(name, age) {}
    Cat() = default;
    Cat(const Cat& rhs) = default;
    Cat& operator=(const Cat& rhs) = default;
    Cat(Cat&& rhs) noexcept = default;
    Cat& operator=(Cat&& rhs) noexcept = default;
    ~Cat() = default;

    /**
     * @brief Makes the cat speak.
     * @details Overrides the base class `speak` method.
     */
    void speak() const {
        std::cout << _name << " says: Meow!" << std::endl;
    }
};
