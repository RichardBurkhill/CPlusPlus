/**
 * @file Observer.cpp
 * @brief Demonstrates the Observer design pattern in C++.
 * @details The Observer pattern is a behavioral design pattern that defines a one-to-many dependency between objects
 *          so that when one object changes state, all its dependents are notified and updated automatically.
 *          It is often used for implementing distributed event handling systems.
 */
#include <vector>    // Required for std::vector to manage observers
#include <iostream>  // Required for std::cout for output in ConcreteObserver

/**
 * @class Observer
 * @brief The abstract base class for all observer objects.
 * @details Defines the interface for objects that should be notified of changes in a Subject.
 *          Concrete observers must implement the update method.
 */
class Observer {
public:
    /**
     * @brief Pure virtual method to be implemented by concrete observers.
     * @details This method is called by the Subject to notify the observer of a state change.
     * @param value The new state value from the Subject.
     */
    virtual void update(int value) = 0;

    /**
     * @brief Virtual destructor to ensure proper cleanup of derived observer classes.
     */
    virtual ~Observer() = default;
};

/**
 * @class Subject
 * @brief The object that maintains a list of its dependents (observers) and notifies them of state changes.
 * @details It is also known as "Observable". When its state changes, it iterates through its registered observers
 *          and calls their update method.
 */
class Subject {
    std::vector<Observer*> observers_; // List of observers attached to this subject
    int state_ = 0;                    // The internal state of the subject

public:
    /**
     * @brief Attaches an observer to the subject.
     * @details Adds an observer to the list of dependents that will be notified of state changes.
     * @param obs A pointer to the Observer object to be attached.
     */
    void attach(Observer* obs) {
        observers_.push_back(obs); // Add the observer to the vector
    }

    /**
     * @brief Sets the state of the subject and notifies all attached observers.
     * @details When the state changes, this method iterates through all registered observers
     *          and calls their update method, passing the new state.
     * @param val The new integer value for the subject's state.
     */
    void setState(int val) {
        state_ = val; // Update the subject's internal state
        // Notify all attached observers about the state change
        for (auto* obs : observers_) {
            obs->update(state_); // Call the update method on each observer
        }
    }
};

/**
 * @class ConcreteObserver
 * @brief A concrete implementation of the Observer interface.
 * @details This observer reacts to updates from the Subject by printing the new state value.
 */
class ConcreteObserver : public Observer {
public:
    /**
     * @brief Implements the update method from the Observer interface.
     * @details This method is called by the Subject when its state changes.
     *          It prints the updated value to the console.
     * @param value The new state value received from the Subject.
     */
    void update(int value) override {
        std::cout << "Updated to " << value << "\n"; // Print the updated value
    }
};
