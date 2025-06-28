#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <algorithm>

// C++11: Strongly typed enumerations
enum class AnimalType { Dog, Cat };

// Base class
class Animal {
public:
    Animal(std::string name) : name_(std::move(name)) {}
    virtual ~Animal() = default;

    // C++11: override keyword
    virtual void speak() const = 0;

    std::string getName() const { return name_; }

protected:
    std::string name_;
};

// Dog class
class Dog final : public Animal {
public:
    Dog(std::string name) : Animal(std::move(name)) {}

    void speak() const override {
        std::cout << name_ << " says: Woof!" << std::endl;
    }
};

// Cat class
class Cat final : public Animal {
public:
    Cat(std::string name) : Animal(std::move(name)) {}

    void speak() const override {
        std::cout << name_ << " says: Meow!" << std::endl;
    }
};

// Zoo class manages animals using smart pointers
class Zoo {
public:
    // C++11: deleted function to avoid copying
    Zoo(const Zoo&) = delete;
    Zoo& operator=(const Zoo&) = delete;

    Zoo() = default;

    // C++11: default move constructor
    Zoo(Zoo&&) = default;
    Zoo& operator=(Zoo&&) = default;

    void addAnimal(std::unique_ptr<Animal> animal) {
        animals_.emplace_back(std::move(animal));
    }

    void makeAllSpeak() const {
        // C++11: range-based for loop
        for (const auto& animal : animals_) {
            animal->speak();
        }
    }

    void listAnimalNames() const {
        std::cout << "Animals in the zoo: ";
        // C++11: lambda
        std::for_each(animals_.begin(), animals_.end(),
                      [](const std::unique_ptr<Animal>& a) {
                          std::cout << a->getName() << " ";
                      });
        std::cout << std::endl;
    }

private:
    std::vector<std::unique_ptr<Animal>> animals_;
};

// C++14: constexpr function
constexpr int doubleAge(int age) {
    return age * 2;
}

int main() {
    Zoo zoo;

    // C++14: make_unique
    zoo.addAnimal(std::make_unique<Dog>("Rex"));
    zoo.addAnimal(std::make_unique<Cat>("Whiskers"));

    zoo.listAnimalNames();
    zoo.makeAllSpeak();

    constexpr int age = 4;
    std::cout << "Double age of " << age << " is " << doubleAge(age) << std::endl;

    return 0;
}
