/**
 * @file C++23.cpp
 * @brief Demonstrates various C++23 language constructs and features.
 *
 * This file showcases key C++23 additions including:
 * - `std::flat_map` for efficient key-value storage.
 * - `std::string::contains` for substring checking.
 * - `std::print` for convenient and safe output formatting.
 * - `if consteval` for compile-time conditional execution.
 * - Deduced `this` for simplified member function overloads.
 */

#include <iostream>
#include <string>
#include <vector>
#include <map>      // For comparison with flat_map
#include <flat_map> // C++23: std::flat_map for contiguous key-value storage.
#include <print>    // C++23: std::print for formatted output.
#include <string_view>

/**
 * @brief A class demonstrating deduced `this` in C++23.
 * @details Deduced `this` allows for more flexible member function overloads
 *          based on the cv-qualifiers and ref-qualifiers of the object.
 */
class MyClass
{
    int value_ = 0;

public:
    /**
     * @brief Constructor for MyClass.
     * @param val Initial value.
     */
    MyClass(int val) : value_(val) {}

    /**
     * @brief Returns the value by const reference.
     * @tparam Self The deduced type of `*this`.
     * @param self The object itself (deduced `this`).
     * @return A const reference to the value.
     * @details This overload is chosen when the object is `const`.
     */
    auto get_value(this const MyClass &self) -> const int &
    { ///< C++23: Deduced `this` for const-qualified member function.
        std::cout << "(const ref) ";
        return self.value_;
    }

    /**
     * @brief Returns the value by non-const reference.
     * @tparam Self The deduced type of `*this`.
     * @param self The object itself (deduced `this`).
     * @return A non-const reference to the value.
     * @details This overload is chosen when the object is non-`const`.
     */
    auto get_value(this MyClass &self) -> int &
    { ///< C++23: Deduced `this` for non-const-qualified member function.
        std::cout << "(non-const ref) ";
        return self.value_;
    }
};

/**
 * @brief A compile-time function demonstrating `if consteval`.
 * @param val An integer value.
 * @return A string indicating if the function is evaluated at compile-time or runtime.
 * @details `if consteval` allows for conditional compilation based on whether
 *          the current evaluation context is a constant evaluation.
 */
const char *check_consteval(int val)
{
    if consteval
    { ///< C++23: if consteval for compile-time conditional execution.
        return "Compile-time evaluation";
    }
    else
    {
        return "Runtime evaluation";
    }
}

/**
 * @brief Main function demonstrating C++23 features.
 *
 * This function showcases:
 * - `std::flat_map` for efficient key-value storage.
 * - `std::string::contains` for easy substring checking.
 * - `std::print` for modern formatted output.
 * - `if consteval` for compile-time branching.
 * - Deduced `this` for flexible member function overloads.
 */
int main()
{
    // C++23: std::flat_map
    std::flat_map<std::string, int> ages; ///< C++23: std::flat_map for contiguous storage and better cache performance.
    ages["Alice"] = 30;
    ages["Bob"] = 25;
    ages["Charlie"] = 35;

    std::print("Ages in flat_map:\n"); ///< C++23: std::print for convenient formatted output.
    for (const auto &[name, age] : ages)
    { // Structured bindings (C++17) work well here.
        std::print("  {}: {}\n", name, age);
    }

    // C++23: std::string::contains
    std::string sentence = "The quick brown fox jumps over the lazy dog.";
    if (sentence.contains("fox"))
    { ///< C++23: std::string::contains for simple substring check.
        std::print("Sentence contains 'fox'.\n");
    }
    if (!sentence.contains("cat"))
    {
        std::print("Sentence does not contain 'cat'.\n");
    }

    // C++23: if consteval
    constexpr int compile_time_val = 10;
    std::print("check_consteval(compile_time_val): {}\n", check_consteval(compile_time_val));

    int runtime_val = 20;
    std::print("check_consteval(runtime_val): {}\n", check_consteval(runtime_val));

    // C++23: Deduced this
    MyClass obj(100);
    const MyClass const_obj(200);

    std::print("obj.get_value(): {}\n", obj.get_value());
    std::print("const_obj.get_value(): {}\n", const_obj.get_value());

    return 0;
}
