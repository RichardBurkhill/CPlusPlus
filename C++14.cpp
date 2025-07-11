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
#include <thread>

#include "Dog.hpp"
#include "Cat.hpp"
#include "Zoo.hpp"
#include "Fib.hpp"

void taskA() {
    for (int i = 0; i < 5; ++i) {
        std::cout << "Task A - " << i << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void taskB() {
    for (int i = 0; i < 5; ++i) {
        std::cout << "Task B - " << i << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
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
    zoo.addAnimal(std::make_unique<Dog>("Rex", 5)); ///< C++14: std::make_unique for safe and efficient unique_ptr creation.
    zoo.addAnimal(std::make_unique<Cat>("Whiskers", 2));

    zoo.listAnimalNames();
    zoo.makeAllSpeak();

    // C++14: constexpr function to double an age
    std::cout << "Using constexpr function to double an age at compile time.\n";
    constexpr int age = 4;
    std::cout << "Double age of " << age << " is " << doubleAge(age) << std::endl;

    // C++14: Generic lambda
    std::vector<Animal> animals = {
        {"Bella", 3},
        {"Charlie", 7},
        {"Max", 2},
        {"Luna", 5}
    };

    std::cout << "All animals:\n";
    std::for_each(animals.begin(), animals.end(), [](const Animal& a) {
        a.speak();
    });

    // C++14: Generic lambda to filter animals older than 4
    std::cout << "\nAnimals older than 4:\n";
    std::vector<Animal> older;
    std::copy_if(animals.begin(), animals.end(), std::back_inserter(older), [](const Animal& a) {
        return a._age > 4;
    });


    for (const auto& a : older) {
        a.speak();
    }

    std::cout << "\nSorting by name:\n";
    std::sort(animals.begin(), animals.end(), [](const Animal& a, const Animal& b) {
        return a._name < b._name;
    });
    std::for_each(animals.begin(), animals.end(), [](const Animal& a) {
        a.speak();
    });

    std::cout << "\nSorting by age:\n";
    std::sort(animals.begin(), animals.end(), [](const Animal& a, const Animal& b) {
        return a._age < b._age;
    });
    std::for_each(animals.begin(), animals.end(), [](const Animal& a) {
        a.speak();
    });

    std::mutex mtx;

    // C++14: Using threads to demonstrate concurrency
    std::cout << "\nUsing threads to demonstrate concurrency:\n";
    std::thread t1([&animals, &mtx]() {
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "\n[Thread 1] Processing animals:\n";
        for (const auto& a : animals) {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            std::cout << "[Thread 1] ";
            a.speak();
        }
    });

    std::thread t2([&animals, &mtx]() {
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "\n[Thread 2] Feeding animals:\n";
        for (const auto& a : animals) {
            std::this_thread::sleep_for(std::chrono::milliseconds(150));
            std::cout << "[Thread 2] Feeding " << a._name << "\n";
        }
    });

    t1.join();
    t2.join();

    std::cout << std::endl;

    std::thread threadA(taskA);  // start taskA on a new thread
    std::thread threadB(taskB);  // start taskB on another new thread

    threadA.join(); // wait for threadA to finish
    threadB.join(); // wait for threadB to finish

    std::cout << "\nFibonacci sequence:\n";
    // C++14: Using a lambda to generate Fibonacci numbers
    std::cout << "First 100 Fibonacci numbers:\n";
    auto fib = generateFibonacci(100);
    std::cout << "Fibonacci sequence:\n";
    for (const auto& num : fib) {
        std::cout << num << "\n";
    }
    std::cout << "End of Fibonacci sequence.\n";

    // C++14: Binary literals and digit separators
    std::cout << "\nUsing binary literals and digit separators:\n";
    int binaryValue = 0b1010'1100; // C++14: Binary literal with digit separators.
    std::cout << "Binary value: " << binaryValue << std::endl; 
    int largeNumber = 1'000'000; // C++14: Digit separator for readability.
    std::cout << "Large number: " << largeNumber << std::endl;
    std::cout << "End of C++14 demonstration." << std::endl;

    return 0;
}
