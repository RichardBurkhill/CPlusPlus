/**
 * @file observer.cpp
 * @brief Demonstrates the Observer pattern in C++.
 * @details Used to notify multiple objects about any events that happen to the object they are observing.
 */
#include <vector>
#include <iostream>

class Observer {
public:
    virtual void update(int value) = 0;
    virtual ~Observer() = default;
};

class Subject {
    std::vector<Observer*> observers_;
    int state_ = 0;
public:
    void attach(Observer* obs) { observers_.push_back(obs); }
    void setState(int val) {
        state_ = val;
        for (auto* obs : observers_) obs->update(state_);
    }
};

class ConcreteObserver : public Observer {
public:
    void update(int value) override {
        std::cout << "Updated to " << value << "\n";
    }
};
