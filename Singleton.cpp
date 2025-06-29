/**
 * @file singleton.cpp
 * @brief Demonstrates the Singleton design pattern in C++.
 * @details Used when exactly one instance of a class is needed globally.
 */
class Singleton {
public:
    /**
     * @brief Gets the single instance of the Singleton.
     * @return Reference to the Singleton instance.
     */
    static Singleton& getInstance() {
        static Singleton instance;
        return instance;
    }

    /**
     * @brief Example method demonstrating use of the Singleton.
     */
    void doSomething() {}

private:
    Singleton() = default;
};