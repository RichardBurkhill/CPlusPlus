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
    return {42, 3.14, "hello"};
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
        {"name", std::string("Alice")},
        {"weight", 68.5}
    };

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
    auto show_type = [](const AttributeValue& val) {
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
