/**
 * @file Decorator.cpp
 * @brief Demonstrates the Decorator pattern in C++.
 * @details The Decorator design pattern is a structural design pattern that allows behavior to be added to an individual object,
 *          either statically or dynamically, without affecting the behavior of other objects from the same class.
 *          Decorators provide a flexible alternative to subclassing for extending functionality.
 *
 *          Key components of the Decorator pattern:
 *          - **Component (interface/abstract class):** Defines the interface for objects that can have responsibilities added to them dynamically.
 *          - **ConcreteComponent:** Implements the Component interface and defines the basic behavior to which additional responsibilities can be attached.
 *          - **Decorator:** Maintains a reference to a Component object and conforms to the Component interface.
 *            It acts as an abstract base class for concrete decorators.
 *          - **ConcreteDecorator:** Extends the functionality of the Component by adding new behavior before or after calling the wrapped Component's operation.
 */
#include <iostream> // Required for input/output operations, specifically std::cout
#include <memory>   // Required for std::unique_ptr, used for managing component ownership

/**
 * @class Component
 * @brief The abstract base class (interface) for objects that can be decorated.
 * @details Defines the common interface for both concrete components and decorators.
 *          This ensures that decorators can be used interchangeably with concrete components.
 */
class Component {
public:
    /**
     * @brief Pure virtual method defining the core operation.
     * @details All concrete components and decorators must implement this method.
     * @return A string representing the result of the operation.
     */
    virtual std::string operation() const = 0;

    /**
     * @brief Virtual destructor to ensure proper cleanup of derived classes.
     * @details Declared as default to allow compiler-generated destructor,
     *          but ensures polymorphism works correctly when deleting Component pointers.
     */
    virtual ~Component() = default;
};

/**
 * @class ConcreteComponent
 * @brief Implements the Component interface, providing the basic behavior.
 * @details This is the object to which additional responsibilities (decorators) can be attached.
 */
class ConcreteComponent : public Component {
public:
    /**
     * @brief Implements the basic operation of the component.
     * @details This is the fundamental behavior that decorators will enhance.
     * @return A string indicating the basic component operation.
     */
    std::string operation() const override {
        return "ConcreteComponent"; // Returns the name of the component
    }
};

/**
 * @class Decorator
 * @brief The abstract base class for all concrete decorators.
 * @details It maintains a reference to a Component object and conforms to the Component interface.
 *          This allows decorators to be chained.
 */
class Decorator : public Component {
protected:
    std::unique_ptr<Component> component_; // Pointer to the wrapped component

public:
    /**
     * @brief Constructor for the Decorator.
     * @param comp A unique pointer to the Component object to be wrapped.
     * @details Initializes the decorator with the component it will decorate.
     *          std::move is used to transfer ownership of the unique_ptr.
     */
    Decorator(std::unique_ptr<Component> comp) : component_(std::move(comp)) {}

    /**
     * @brief Delegates the operation call to the wrapped component.
     * @details This method can be overridden by concrete decorators to add behavior
     *          before or after calling the wrapped component's operation.
     * @return A string representing the decorated operation.
     */
    std::string operation() const override {
        // Delegates the call to the wrapped component's operation
        return component_->operation();
    }
};

/**
 * @class ConcreteDecoratorA
 * @brief A concrete decorator that adds specific behavior to the component.
 * @details This decorator adds "DecoratorA" to the operation's output.
 */
class ConcreteDecoratorA : public Decorator {
public:
    /**
     * @brief Constructor for ConcreteDecoratorA.
     * @param comp A unique pointer to the Component object to be decorated.
     * @details Passes the component to the base Decorator constructor.
     */
    ConcreteDecoratorA(std::unique_ptr<Component> comp) : Decorator(std::move(comp)) {}

    /**
     * @brief Overrides the operation to add new behavior.
     * @details Calls the base decorator's operation (which in turn calls the wrapped component's operation)
     *          and then adds its own specific decoration.
     * @return A string representing the operation decorated by ConcreteDecoratorA.
     */
    std::string operation() const override {
        // Adds "DecoratorA" around the result of the wrapped component's operation
        return "DecoratorA(" + Decorator::operation() + ")";
    }
};

/**
 * @class ConcreteDecoratorB
 * @brief Another concrete decorator that adds different behavior.
 * @details This decorator adds "DecoratorB" to the operation's output.
 */
class ConcreteDecoratorB : public Decorator {
public:
    /**
     * @brief Constructor for ConcreteDecoratorB.
     * @param comp A unique pointer to the Component object to be decorated.
     * @details Passes the component to the base Decorator constructor.
     */
    ConcreteDecoratorB(std::unique_ptr<Component> comp) : Decorator(std::move(comp)) {}

    /**
     * @brief Overrides the operation to add new behavior.
     * @details Calls the base decorator's operation and then adds its own specific decoration.
     * @return A string representing the operation decorated by ConcreteDecoratorB.
     */
    std::string operation() const override {
        // Adds "DecoratorB" around the result of the wrapped component's operation
        return "DecoratorB(" + Decorator::operation() + ")";
    }
};

// Example of how to use the Decorator pattern
/*
int main() {
    // 1. Create a basic component
    std::unique_ptr<Component> simpleComponent = std::make_unique<ConcreteComponent>();
    std::cout << "Client: I've got a simple component: " << simpleComponent->operation() << std::endl;

    // 2. Decorate the simple component with DecoratorA
    std::unique_ptr<Component> decoratedComponentA = std::make_unique<ConcreteDecoratorA>(std::move(simpleComponent));
    std::cout << "Client: Now I've got a decorated component A: " << decoratedComponentA->operation() << std::endl;

    // 3. Decorate the already decorated component (DecoratorA) with DecoratorB
    // This demonstrates chaining decorators
    std::unique_ptr<Component> decoratedComponentAB = std::make_unique<ConcreteDecoratorB>(std::move(decoratedComponentA));
    std::cout << "Client: And now a doubly decorated component AB: " << decoratedComponentAB->operation() << std::endl;

    // You can also decorate the original simple component directly with DecoratorB
    std::unique_ptr<Component> simpleComponent2 = std::make_unique<ConcreteComponent>();
    std::unique_ptr<Component> decoratedComponentB = std::make_unique<ConcreteDecoratorB>(std::move(simpleComponent2));
    std::cout << "Client: Or just decorated with B: " << decoratedComponentB->operation() << std::endl;

    return 0;
}
*/
