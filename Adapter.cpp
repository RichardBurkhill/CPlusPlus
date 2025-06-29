/**
 * @file Adapter.cpp
 * @brief Demonstrates the Adapter design pattern in C++.
 * @details The Adapter pattern allows incompatible interfaces to work together.
 *          It acts as a wrapper between two objects, translating calls from one
 *          interface to another.
 */

#include <iostream>

/**
 * @class OldInterface
 * @brief Represents an existing class with an incompatible interface.
 * @details This class has a `specificRequest` method that clients cannot directly use
 *          because they expect a different interface.
 */
class OldInterface
{
public:
    /**
     * @brief Performs a specific request using the old interface.
     * @details This method is part of the legacy system and needs to be adapted
     *          to fit the `NewInterface` expectation.
     */
    void specificRequest() const
    {
        std::cout << "OldInterface: Performing specific request.\n"; ///< Output indicating the old interface's action.
    }
};

/**
 * @class NewInterface
 * @brief Defines the target interface that clients expect.
 * @details This is an abstract base class with a pure virtual `request` method,
 *          which all concrete implementations (including adapters) must provide.
 */
class NewInterface
{
public:
    /**
     * @brief Pure virtual function for the new interface's request.
     * @details This method must be implemented by concrete classes that conform
     *          to the `NewInterface`.
     */
    virtual void request() const = 0;

    /**
     * @brief Virtual destructor to ensure proper cleanup of derived classes.
     */
    virtual ~NewInterface() = default;
};

/**
 * @class Adapter
 * @brief Adapts the `OldInterface` to conform to the `NewInterface`.
 * @details This class inherits from `NewInterface` and contains an instance of
 *          `OldInterface`. It translates calls from `request()` to `specificRequest()`.
 */
class Adapter : public NewInterface
{
private:
    OldInterface old_interface_instance_; ///< @brief An instance of the `OldInterface` to be adapted.

public:
    /**
     * @brief Default constructor for the Adapter.
     * @details Initializes the internal `OldInterface` instance.
     */
    Adapter() = default;

    /**
     * @brief Implements the `request` method of `NewInterface`.
     * @details This method delegates the call to the `specificRequest` method
     *          of the `OldInterface` instance, effectively adapting the call.
     */
    void request() const override
    {
        std::cout << "Adapter: Translating request to old interface... "; ///< Informative message about the adaptation.
        old_interface_instance_.specificRequest();                        ///< Calls the method of the adapted class.
    }
};

/**
 * @brief Main function to demonstrate the Adapter pattern.
 * @details Creates an `Adapter` object and calls its `request` method,
 *          showing how the `OldInterface` functionality is accessed through
 *          the `NewInterface`.
 */
int main()
{
    std::cout << "Client: Expecting NewInterface.\n"; ///< Client's perspective.

    // The client code works with the NewInterface
    NewInterface *adapter = new Adapter(); ///< @brief Create an Adapter object, treated as a NewInterface.
    adapter->request();                    ///< @brief Call the request method, which is handled by the Adapter.

    delete adapter; ///< @brief Clean up dynamically allocated memory.
    return 0;
}
