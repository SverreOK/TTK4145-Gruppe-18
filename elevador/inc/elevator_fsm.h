#pragma once
#ifndef ELEVATOR_FSM_H
#define ELEVATOR_FSM_H

// EVENTS
enum class ElevatorEvent {
    BUTTON_PRESSED_UP,
    BUTTON_PRESSED_DOWN,
    ARRIVED_AT_FLOOR,
    DOOR_TIMER_EXPIRED,
    STOP_BUTTON_PRESSED,
    STOP_BUTTON_TIMEOUT,
    VALID_FLOOR_FOUND,
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
    ElevatorState currentState;

public:

    void entryState(ElevatorState state);
    void exitState(ElevatorState state);
    void handleEvent(ElevatorEvent event);

    void handleEventInit(ElevatorEvent event);
    void handleEventIdle(ElevatorEvent event);
    void handleEventMovingUp(ElevatorEvent event);
    void handleEventMovingDown(ElevatorEvent event);
    void handleEventStop(ElevatorEvent event);

    Elevator();

    void updateState(ElevatorEvent event);
};

#endif /* ELEVATOR_FSM_H */