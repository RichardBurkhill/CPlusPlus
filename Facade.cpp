/**
 * @file Facade.cpp
 * @brief Demonstrates the Facade pattern in C++.
 * @details The Facade design pattern is a structural design pattern that provides a simplified interface
 *          to a complex subsystem of classes, libraries, or frameworks. It hides the complexities of the
 *          subsystem and provides a single, unified, and easy-to-use interface to the client.
 *
 *          Key components of the Facade pattern:
 *          - **Facade:** Provides a simplified, high-level interface to the subsystem. It knows which subsystem
 *            classes are responsible for a request and delegates client requests to appropriate subsystem objects.
 *          - **Subsystem Classes:** Implement the complex functionality. They are not aware of the facade and
 *            don't keep references to it. They perform the actual work.
 *          - **Client:** Uses the Facade to interact with the subsystem. It doesn't directly communicate with
 *            the subsystem classes.
 */
#include <iostream> // Required for input/output operations, specifically std::cout

/**
 * @class SubsystemA
 * @brief Represents one part of a complex subsystem.
 * @details This class provides a specific functionality that is part of the larger system.
 *          The Facade will interact with this class to perform a high-level operation.
 */
class SubsystemA {
public:
    /**
     * @brief Performs operation A.
     * @details This method represents a specific task within Subsystem A.
     */
    void doA() const { 
        std::cout << "SubsystemA: Performing operation A\n"; // Output indicating operation A
    }
};

/**
 * @class SubsystemB
 * @brief Represents another part of a complex subsystem.
 * @details This class provides a different functionality that complements SubsystemA.
 *          The Facade will also interact with this class.
 */
class SubsystemB {
public:
    /**
     * @brief Performs operation B.
     * @details This method represents a specific task within Subsystem B.
     */
    void doB() const { 
        std::cout << "SubsystemB: Performing operation B\n"; // Output indicating operation B
    }
};

/**
 * @class Facade
 * @brief Provides a simplified interface to the complex subsystem (SubsystemA and SubsystemB).
 * @details This class encapsulates the interactions with the various subsystem components,
 *          making it easier for clients to use the functionality without knowing the internal details.
 */
class Facade {
    SubsystemA a_; // Instance of SubsystemA, part of the complex subsystem
    SubsystemB b_; // Instance of SubsystemB, another part of the complex subsystem

public:
    /**
     * @brief Performs a high-level operation by coordinating calls to subsystem classes.
     * @details This method simplifies the client's interaction by abstracting away the need
     *          to call SubsystemA::doA() and SubsystemB::doB() separately.
     */
    void operation() {
        std::cout << "Facade: Initiating complex operation...\n"; // Informative message from the Facade
        a_.doA(); // Delegates to SubsystemA
        b_.doB(); // Delegates to SubsystemB
        std::cout << "Facade: Complex operation completed.\n"; // Informative message from the Facade
    }
};

// Example of how to use the Facade pattern
/*
int main() {
    Facade facade; // Create an instance of the Facade

    // The client interacts only with the Facade, not directly with SubsystemA or SubsystemB
    facade.operation(); // Call the simplified operation on the Facade

    return 0;
}
*/
