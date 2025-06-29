#include <iostream>
#include <vector>
#include <ranges>        // Ranges library
#include <concepts>      // Concepts
#include <numbers>       // Math constants like std::numbers::pi
#include <coroutine>     // Coroutines
#include <optional>
#include <format>        // Requires C++20 + libc++20 or libstdc++11+
#include <span>          // For safe view over arrays
#include <type_traits>

// --------------------------
// Concept: Numeric check
template<typename T>
concept Numeric = std::is_arithmetic_v<T>;

// Function constrained with a concept (C++20)
Numeric auto square(Numeric auto x) {
    return x * x;
}

// Coroutine generator
template<typename T>
struct Generator {
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;

    struct promise_type {
        T current_value;
        auto get_return_object() { return Generator{handle_type::from_promise(*this)}; }
        auto initial_suspend() { return std::suspend_always{}; }
        auto final_suspend() noexcept { return std::suspend_always{}; }
        void unhandled_exception() { std::exit(1); }
        auto yield_value(T value) {
            current_value = value;
            return std::suspend_always{};
        }
        void return_void() {}
    };

    handle_type coro;
    Generator(handle_type h) : coro(h) {}
    ~Generator() { if (coro) coro.destroy(); }

    T next() {
        coro.resume();
        return coro.done() ? T{} : coro.promise().current_value;
    }
};

Generator<int> counter(int max) {
    for (int i = 0; i <= max; ++i)
        co_yield i;
}

int main() {
    std::cout << std::format("Square of {} is {}\n", 5, square(5));
    std::cout << std::format("π squared is {:.5f}\n", square(std::numbers::pi));

    // Ranges: filter + transform
    std::vector<int> nums{1, 2, 3, 4, 5, 6};
    auto even_squares = nums | std::views::filter([](int n) { return n % 2 == 0; })
                             | std::views::transform([](int n) { return n * n; });

    std::cout << "Even squares: ";
    for (int x : even_squares)
        std::cout << x << " ";
    std::cout << "\n";

    // Span (non-owning view)
    int raw_array[] = {10, 20, 30, 40};
    std::span<int> view(raw_array);
    std::cout << "Span contents: ";
    for (int x : view)
        std::cout << x << " ";
    std::cout << "\n";

    // Coroutines: simple counter
    std::cout << "Counting to 5 using coroutine:\n";
    auto gen = counter(5);
    for (int i = 0; i <= 5; ++i)
        std::cout << gen.next() << " ";

    std::cout << "\n";
    return 0;
}

