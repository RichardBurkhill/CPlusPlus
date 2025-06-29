/**
 * @file Singleton.cpp
 * @brief Demonstrates the Singleton design pattern in C++.
 * @details The Singleton design pattern ensures that a class has only one instance and provides a global point
 *          of access to that instance. It is useful when exactly one object is needed to coordinate actions
 *          across the system.
 */

/**
 * @class Singleton
 * @brief Implements the Singleton design pattern.
 * @details This class ensures that only one instance of itself can be created and provides a static method
 *          to get that single instance. The constructor is private to prevent direct instantiation.
 */
class Singleton {
public:
    /**
     * @brief Gets the single instance of the Singleton.
     * @details This static method provides the global access point to the Singleton instance.
     *          It uses a static local variable to ensure that the instance is created only once
     *          (thread-safe in C++11 and later).
     * @return Reference to the Singleton instance.
     */
    static Singleton& getInstance() {
        static Singleton instance; // Guaranteed to be destroyed correctly at program termination
        return instance;
    }

    /**
     * @brief Example method demonstrating use of the Singleton.
     * @details This method can be called on the single instance of the Singleton to perform some action.
     */
    void doSomething() {
        // Example implementation of a method
    }

    // Delete copy constructor and assignment operator to prevent copying
    /**
     * @brief Deleted copy constructor.
     * @details Prevents the creation of new Singleton instances via copying.
     */
    Singleton(const Singleton&) = delete;

    /**
     * @brief Deleted assignment operator.
     * @details Prevents assignment of Singleton instances.
     */
    Singleton& operator=(const Singleton&) = delete;

private:
    /**
     * @brief Private default constructor.
     * @details Prevents direct instantiation of the Singleton class from outside.
     */
    Singleton() = default;

    /**
     * @brief Private destructor.
     * @details While not strictly necessary for basic Singleton, defining it as private
     *          can sometimes be part of more complex Singleton implementations (e.g., for specific cleanup).
     *          However, for the common static local variable approach, it's usually default.
     */
    ~Singleton() = default;
};
