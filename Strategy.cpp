/**
 * @file Strategy.cpp
 * @brief Demonstrates the Strategy design pattern in C++.
 * @details The Strategy pattern is a behavioral design pattern that enables selecting an algorithm at runtime.
 *          Instead of implementing a single algorithm directly, the client code receives run-time instructions
 *          as to which algorithm to use from a family of algorithms. It defines a family of algorithms,
 *          encapsulates each one, and makes them interchangeable. Strategy lets the algorithm vary independently
 *          from clients that use it.
 */
#include <iostream> // Required for std::cout for output in ConcreteStrategyA
#include <memory>   // Required for std::unique_ptr for managing strategy objects

/**
 * @class Strategy
 * @brief The abstract base class for all strategy algorithms.
 * @details Defines the interface for algorithms that can be used by the Context. Concrete strategies
 *          must implement the execute method.
 */
class Strategy {
public:
    /**
     * @brief Pure virtual method to be implemented by concrete strategies.
     * @details This method defines the common interface for all algorithms. Each concrete strategy
     *          will provide its own implementation of this method.
     */
    virtual void execute() const = 0;

    /**
     * @brief Virtual destructor to ensure proper cleanup of derived strategy classes.
     * @details A virtual destructor is essential when dealing with polymorphic base classes to ensure
     *          that the correct destructor is called when an object is deleted via a base class pointer.
     */
    virtual ~Strategy() = default;
};

/**
 * @class ConcreteStrategyA
 * @brief A concrete implementation of the Strategy interface.
 * @details This class provides a specific algorithm implementation for the Strategy interface.
 *          It demonstrates one way an operation can be performed.
 */
class ConcreteStrategyA : public Strategy {
public:
    /**
     * @brief Implements the execute method from the Strategy interface.
     * @details This specific implementation prints a message indicating that Strategy A is being executed.
     */
    void execute() const override { std::cout << "Executing Strategy A\n"; } // Specific algorithm implementation
};

/**
 * @class Context
 * @brief The class that uses a Strategy object to perform an action.
 * @details The Context maintains a reference to a Strategy object and delegates the execution of the algorithm
 *          to the linked Strategy object. It does not know the concrete type of the strategy, only its interface.
 */
class Context {
    std::unique_ptr<Strategy> strategy_; // Pointer to the current strategy object
public:
    /**
     * @brief Constructor for the Context class.
     * @details Initializes the Context with a specific strategy. The Context takes ownership of the strategy object.
     * @param s A unique pointer to a Strategy object.
     */
    Context(std::unique_ptr<Strategy> s) : strategy_(std::move(s)) {} // Takes ownership of the strategy

    /**
     * @brief Executes the algorithm defined by the current strategy.
     * @details Delegates the call to the execute method of the currently set strategy object.
     *          This allows the Context to perform an action without knowing the concrete algorithm.
     */
    void run() const { strategy_->execute(); } // Delegates the execution to the strategy
};
