#include <iostream>
#include <string>
#include <map>
#include <variant>
#include <optional>
#include <tuple>
#include <filesystem>

// C++17: structured bindings, if constexpr, std::optional, std::variant, filesystem

// Define a variant type (C++17)
using AttributeValue = std::variant<int, double, std::string>;

// Function that returns an optional (C++17)
std::optional<AttributeValue> get_attribute(const std::map<std::string, AttributeValue>& attrs, const std::string& key) {
    if (auto it = attrs.find(key); it != attrs.end()) {  // C++17: if with initializer
        return it->second;
    } else {
        return std::nullopt;
    }
}

// Visitor for variant (C++17)
void print_attribute(const AttributeValue& value) {
    std::visit([](auto&& val) {
        std::cout << "Value: " << val << "\n";
    }, value);
}

// Tuple and structured bindings (C++17)
std::tuple<int, double, std::string> get_tuple_data() {
    return {42, 3.14, "hello"};
}

int main() {
    std::map<std::string, AttributeValue> attributes = {
        {"id", 123},
        {"name", std::string("Alice")},
        {"weight", 68.5}
    };

    // Optional use
    if (auto attr = get_attribute(attributes, "name")) {
        print_attribute(*attr);
    } else {
        std::cout << "Attribute not found\n";
    }

    // Structured bindings (C++17)
    auto [a, b, c] = get_tuple_data();
    std::cout << "Tuple unpacked: " << a << ", " << b << ", " << c << "\n";

    // Filesystem example (C++17)
    std::filesystem::path current = std::filesystem::current_path();
    std::cout << "Current path: " << current << "\n";

    // constexpr if (C++17)
    auto show_type = [](const AttributeValue& val) {
        std::visit([](auto&& v) {
            if constexpr (std::is_same_v<decltype(v), int>)
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

