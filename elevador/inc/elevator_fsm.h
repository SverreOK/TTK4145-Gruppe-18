#pragma once
#ifndef ELEVATOR_FSM_H
#define ELEVATOR_FSM_H
#define BOOST_THREAD_USE_LIB
// INCLUDES
#include "elevator_driver.h"
#include "call-class.h"
#include <mutex>

// EVENTS
enum class ElevatorEvent {
    ORDER_RECEIVED,
    STOP_BUTTON_PRESSED,
    ARRIVED_AT_FLOOR,
    OBSTRUCTION
};

// STATES
enum class ElevatorState {
    INIT,
    IDLE,
    MOVING_UP,
    MOVING_DOWN,
    STOP
};

class Elevator {
private:
    ElevatorDriver* driver;
    ElevatorState currentState;
    int8_t currentFloor;
    
    uint8_t stopButton;
    uint8_t obstruction;
    
    std::mutex eventMutex;
    

public:

    void updateState(ElevatorState state);
    void entryState(ElevatorState state);
    void handleEvent(ElevatorEvent event);

    // Floor
    void setFloor(int8_t floor);
    int8_t getFloor();
    void floorPoller();

    // Stop button
    void pollStopButton();
    void setStopButtonLight(uint8_t);

    // Obstruction
    void pollObstruction();

    //algorithms
    bool shouldStop();


    Elevator(ElevatorDriver* driver, ElevatorState initialState, uint8_t currentFloor);
    ~Elevator();
};

#endif /* ELEVATOR_FSM_H */