/**
 * @file Command.cpp
 * @brief Demonstrates the Command pattern in C++.
 * @details The Command design pattern is a behavioral design pattern that turns a request into a stand-alone object.
 *          This allows for parameterizing clients with different requests, queuing requests, logging requests,
 *          and supporting undoable operations. It decouples the sender of a request from the object that performs the request.
 *
 *          Key components of the Command pattern:
 *          - **Command (interface/abstract class):** Declares an interface for executing an operation.
 *          - **ConcreteCommand:** Implements the Command interface and binds a Receiver object with an action.
 *            It defines a link between the Receiver and the action.
 *          - **Client:** Creates a ConcreteCommand object and sets its Receiver.
 *          - **Invoker:** Asks the command to carry out the request. It holds a Command object.
 *          - **Receiver:** Knows how to perform the operations associated with carrying out a request.
 *            Any class can act as a Receiver.
 */
#include <iostream> // Required for input/output operations, specifically std::cout

/**
 * @class Command
 * @brief The abstract base class for all command objects.
 * @details This interface declares a method for executing a command.
 *          All concrete command classes must implement this interface.
 */
class Command {
public:
    /**
     * @brief Pure virtual method to execute the command.
     * @details Concrete command classes will implement this method to perform a specific action
     *          on a receiver.
     */
    virtual void execute() = 0;

    /**
     * @brief Virtual destructor to ensure proper cleanup of derived classes.
     * @details Declared as default to allow compiler-generated destructor,
     *          but ensures polymorphism works correctly.
     */
    virtual ~Command() = default;
};

/**
 * @class Receiver
 * @brief The class that knows how to perform the actual operations.
 * @details This class contains the business logic that the command will delegate to.
 *          It's the object that performs the actual work when the command is executed.
 */
class Receiver {
public:
    /**
     * @brief Performs a specific action.
     * @details This method represents the actual operation that the command encapsulates.
     *          In a real application, this would contain the core business logic.
     */
    void action() {
        std::cout << "Action executed\n"; // Output indicating the action has been performed
    }
};

/**
 * @class ConcreteCommand
 * @brief Implements the Command interface and binds a Receiver object with an action.
 * @details This class encapsulates a specific request by coupling a receiver with an action.
 *          It implements the `execute` method by calling the corresponding action on the receiver.
 */
class ConcreteCommand : public Command {
    Receiver& receiver_; // Reference to the receiver object that will perform the action

public:
    /**
     * @brief Constructor for ConcreteCommand.
     * @param r A reference to the Receiver object that this command will operate on.
     * @details Initializes the command with a specific receiver.
     */
    ConcreteCommand(Receiver& r) : receiver_(r) {}

    /**
     * @brief Executes the command by delegating the call to the receiver's action method.
     * @details This method is the implementation of the pure virtual `execute` method from the
     *          `Command` base class. It invokes the specific operation on the associated receiver.
     */
    void execute() override {
        receiver_.action(); // Delegates the execution to the receiver's action method
    }
};

// Example of how to use the Command pattern (Invoker and Client would be here)
/*
int main() {
    Receiver receiver; // The object that will perform the action
    ConcreteCommand command(receiver); // Create a concrete command, linking it to the receiver

    // In a real scenario, an Invoker would hold and execute the command
    // For demonstration, we directly execute the command
    command.execute(); // Execute the command, which in turn calls receiver.action()

    return 0;
}
*/
