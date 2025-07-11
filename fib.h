#include <vector>

std::vector<unsigned long long> generateFibonacci(int n)
{
    std::vector<unsigned long long> fib(n);
    unsigned long long a = 0, b = 1;
    std::generate(fib.begin(), fib.begin() + n, [&]() {
        auto next = a;
        std::tie(a, b) = std::make_pair(b, a + b);
        return next;
    });
    return fib;
}