#include "Animal.hpp"
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
