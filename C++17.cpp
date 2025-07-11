/**
 * @file C++17.cpp
 * @brief Demonstrates various C++17 language constructs and features.
 *
 * This file showcases key C++17 additions including:
 * - Structured Bindings
 * - `if constexpr`
 * - `std::optional`
 * - `std::variant`
 * - `std::filesystem`
 * - `if` statement with initializer
 * - `std::visit` for `std::variant`
 */

#include <iostream>
#include <string>
#include <map>
#include <variant>    ///< C++17: std::variant for type-safe polymorphic values.
#include <optional>   ///< C++17: std::optional for representing optional values.
#include <tuple>
#include <filesystem> ///< C++17: std::filesystem for file system operations.
#include <string_view> ///< C++17: std::string_view for lightweight string handling.
#include <vector>
#include <algorithm>  ///< C++17: std::generate for generating sequences.
#include <execution>  ///< C++17: Parallel execution policies for algorithms.
#include <type_traits> ///< C++17: std::is_same_v for compile-time type checking.

// C++17: structured bindings, if constexpr, std::optional, std::variant, filesystem

/**
 * @brief Defines a variant type for attribute values.
 *
 * This `std::variant` can hold an `int`, `double`, or `std::string`,
 * demonstrating C++17's type-safe union.
 */
using AttributeValue = std::variant<int, double, std::string>;

/**
 * @brief Retrieves an attribute value from a map, returning an optional.
 *
 * @param attrs The map of attribute names to their `AttributeValue`.
 * @param key The name of the attribute to retrieve.
 * @return An `std::optional<AttributeValue>` containing the attribute's value
 *         if found, or `std::nullopt` if not.
 * @note Uses C++17's `if` statement with an initializer (`if (auto it = ...; it != ...)`)
 *       for concise map lookup.
 */
std::optional<AttributeValue> get_attribute(const std::map<std::string, AttributeValue>& attrs, const std::string& key) {
    if (auto it = attrs.find(key); it != attrs.end()) {  ///< C++17: if with initializer for concise scope.
        return it->second;
    } else {
        return std::nullopt; ///< C++17: std::nullopt to indicate no value.
    }
}

/**
 * @brief Prints the value held by an `AttributeValue` using `std::visit`.
 *
 * @param value The `AttributeValue` to print.
 * @note Employs C++17's `std::visit` with a generic lambda to handle different
 *       types stored in the `std::variant`.
 */
void print_attribute(const AttributeValue& value) {
    std::visit([](auto&& val) { ///< C++17: std::visit for dispatching on variant types.
        std::cout << "Value: " << val << "\n";
    }, value);
}

/**
 * @brief Returns a `std::tuple` containing mixed data types.
 *
 * @return A `std::tuple` with an `int`, `double`, and `std::string`.
 */
std::tuple<int, double, std::string> get_tuple_data() {
    return {47, 3.141592, "hello"};
}

void greet(std::string_view name) {
    std::cout << "Hello, " << name << "!\n";
}

std::optional<int> maybeDivide(int a, int b) {
    if (b == 0) return std::nullopt;
    return a / b;
}

/**
 * @brief Main function demonstrating C++17 features.
 *
 * This function initializes a map with `std::variant` values,
 * demonstrates `std::optional` usage, structured bindings,
 * `std::filesystem` operations, and `if constexpr`.
 */
int main() {
    std::map<std::string, AttributeValue> attributes = {
        {"id", 123},
        {"name", std::string("John Doe")},
        {"height", 1.75 }, ///< C++17: Using double for height.
        {"age", 30}, ///< C++17: Using int for age.
        {"weight", 68.5}
    };

    //std::variant<int, std::string> data;
    AttributeValue data; ///< C++17: Using std::variant to hold different types.
    data = std::string("Richard");
    std::visit([](auto&& val) { std::cout << "Holds: " << val << "\n"; }, data);
    greet("Richard"); ///< C++17: Using std::string_view for efficient string handling.

    data = 47;
    std::visit([](auto&& val) { std::cout << "Holds: " << val << "\n"; }, data);

    // Using std::optional to handle optional attributes
    std::cout << "Optional attribute example:\n";
    auto optional_value = maybeDivide(10, 0); ///< C++17: std::optional for optional return values.
    if (optional_value) {
        std::cout << "Result: " << *optional_value << "\n";
    } else {
        std::cout << "Division by zero!\n";
    }   
    attributes["name"] = std::string("Richard Burkhill"); ///< C++17: Using std::string for string attributes.
    // Optional use
    if (auto attr = get_attribute(attributes, "name")) { ///< C++17: if with initializer for std::optional.
        print_attribute(*attr);
    } else {
        std::cout << "Attribute not found\n";
    }

    // Structured bindings (C++17)
    auto [a, b, c] = get_tuple_data(); ///< C++17: Structured bindings to unpack tuple elements directly.
    std::cout << "Tuple unpacked: " << a << ", " << b << ", " << c << "\n";

    // Filesystem example (C++17)
    std::filesystem::path p = "/Users/richardburkhill/dev/CPlusPlus/main.cpp";
    std::cout << "Filename: " << p.filename() << "\n";
    std::cout << "Path: " << p.parent_path() << "\n";
    std::cout << "Path exists: " << std::filesystem::exists(p) << "\n";
    std::cout << "Path is a directory: " << std::filesystem::is_directory(p) << "\n";
    std::cout << "Path is a file: " << std::filesystem::is_regular_file(p) << "\n";
    std::filesystem::path current = std::filesystem::current_path(); ///< C++17: std::filesystem for path manipulation.
    std::cout << "Current path: " << current << "\n";

    // constexpr if (C++17)
    /**
     * @brief Lambda function demonstrating `if constexpr` for compile-time type checking.
     *
     * This lambda uses `std::visit` on an `AttributeValue` and then
     * `if constexpr` to perform compile-time branching based on the
     * actual type held by the variant.
     */
    constexpr auto show_type = [](const AttributeValue& val) {
        std::visit([](auto&& v) {
            if constexpr (std::is_same_v<decltype(v), int>) ///< C++17: if constexpr for compile-time conditional compilation.
                std::cout << "It's an int.\n";
            else if constexpr (std::is_same_v<decltype(v), double>)
                std::cout << "It's a double.\n";
            else if constexpr (std::is_same_v<decltype(v), std::string>)
                std::cout << "It's a string.\n";
        }, val);
    };

    show_type(attributes["weight"]);

    return 0;
}
