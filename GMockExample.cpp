#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <string>
#include <utility>
#include <functional>
#include <stdexcept>
#include <sstream>

#include "../CPlusPlus/Dog.hpp"
#include "../CPlusPlus/Cat.hpp"
#include "../CPlusPlus/Zoo.hpp"
#include "../CPlusPlus/Fib.hpp"

// Capture std::cout output for testable output comparison
class CoutRedirector {
public:
    CoutRedirector(std::stringstream& ss) : old_buf(std::cout.rdbuf(ss.rdbuf())) {}
    ~CoutRedirector() { std::cout.rdbuf(old_buf); }
private:
    std::streambuf* old_buf;
};

TEST(AnimalTest, SpeakOutputsNameAndAge) {
    Animal a("TestAnimal", 3);
    std::stringstream buffer;
    CoutRedirector redirect(buffer);

    a.speak();

    std::string expected = "TestAnimal says hello, age 3\n";
    EXPECT_EQ(buffer.str(), expected);
}

TEST(DogTest, SpeakOutputsWoof) {
    Dog d("Rex", 5);
    std::stringstream buffer;
    CoutRedirector redirect(buffer);

    d.speak();

    EXPECT_EQ(buffer.str(), "Rex says: Woof!\n");
}

TEST(CatTest, SpeakOutputsMeow) {
    Cat c("Whiskers", 2);
    std::stringstream buffer;
    CoutRedirector redirect(buffer);

    c.speak();

    EXPECT_EQ(buffer.str(), "Whiskers says: Meow!\n");
}

TEST(ZooTest, AddAnimalsAndListNames) {
    Zoo zoo;
    zoo.addAnimal(std::make_unique<Dog>("Buddy", 4));
    zoo.addAnimal(std::make_unique<Cat>("Mittens", 2));

    std::stringstream buffer;
    CoutRedirector redirect(buffer);
    zoo.listAnimalNames();

    EXPECT_EQ(buffer.str(), "Animals in the zoo: Buddy Mittens \n");
}

TEST(ConstexprTest, DoubleAgeCompileTime) {
    constexpr int input = 6;
    constexpr int expected = 12;
    static_assert(doubleAge(input) == expected, "doubleAge should return double the input");
    EXPECT_EQ(doubleAge(input), expected);
}

TEST(FibonacciTest, GenerateFirstFew) {
    std::vector<unsigned long long> fib(5);
    unsigned long long a = 0, b = 1;
    std::generate(fib.begin(), fib.end(), [&]() {
        auto next = a;
        std::tie(a, b) = std::make_pair(b, a + b);
        return next;
    });

    std::vector<unsigned long long> expected = {0, 1, 1, 2, 3};
    EXPECT_EQ(fib, expected);
}

TEST(FibonacciTest, GenerateUpToN) {
    unsigned long long n = 10;
    std::vector<unsigned long long> fib = generateFibonacci(n);
    std::vector<unsigned long long> expected = {0, 1, 1, 2, 3, 5, 8, 13, 21, 34};

    EXPECT_EQ(fib.size(), expected.size());
    for (size_t i = 0; i < fib.size(); ++i) {
        EXPECT_EQ(fib[i], expected[i]);
    }
}
