#include "inc/elevator_fsm.h"
#include "inc/elevator_driver.h"

class Elevator {
    private:
        ElevatorState currentState;

    public:
        Elevator() : currentState(ElevatorState::INIT) {}

        void updateState(ElevatorState nextState) {
            exitState(currentState);

            currentState = nextState;

            entryState(currentState);
        }

        // ENTRY TASKS
        void entryState(ElevatorState state) {
            switch (state)
            {
            case ElevatorState::INIT:
                // init elevator stuff
                // find valid state/floor?
                break;
            case ElevatorState::IDLE:
                // open door with timer
                // close door if no obstruction
                break;
            case ElevatorState::MOVING_UP:
                // elevator direction up
                break;
            case ElevatorState::MOVING_DOWN:
                // elevator direction down
                break;
            case ElevatorState::STOP:
                // stop motor
                // stop button light
                // if valid floor open door with timer
                // close door if no obstruction
                break;
            default:
                break;
            }
        }

        // EXIT TASKS
        void exitState(ElevatorState state) {
            switch (state)
            {
            case ElevatorState::INIT:
                break;
            case ElevatorState::IDLE:
                // close door if no obstruction
                break;
            case ElevatorState::MOVING_UP:
                // stop motor
                // reached order?
                break;
            case ElevatorState::MOVING_DOWN:
                // stop motor
                // reached order?
                break;
            case ElevatorState::STOP:
                // ??
                break;
            default:
                break;
            }
        }

        // EVENT HANDLING
        void handleEvent(ElevatorEvent event) {
            switch(currentState) {
                case ElevatorState::INIT:
                    handleEventInit(event);
                    break;
                case ElevatorState::IDLE:
                    handleEventIdle(event);
                    break;
                case ElevatorState::MOVING_UP:
                    handleEventMovingUp(event);
                    break;
                case ElevatorState::MOVING_DOWN:
                    handleEventMovingDown(event);
                    break;
                case ElevatorState::STOP:
                    handleEventStop(event);
                    break;
            }
        }

        void handleEventInit(ElevatorEvent event) {
            //no events will be handled in init
        }
        void handleEventIdle(ElevatorEvent event) {
            switch (event)
            {
            case ElevatorEvent::BUTTON_PRESSED_UP:
                updateState(ElevatorState::MOVING_UP);
                break;
            case ElevatorEvent::BUTTON_PRESSED_DOWN:
                updateState(ElevatorState::MOVING_DOWN);
                break;
            case ElevatorEvent::STOP_BUTTON_PRESSED:
                updateState(ElevatorState::STOP);
                break;
            
            default:
                break;
            }
        }
        void handleEventMovingDown(ElevatorEvent event) {
            switch (event)
            {
            case ElevatorEvent::BUTTON_PRESSED_UP:
                updateState(ElevatorState::MOVING_UP);
                break;
            case ElevatorEvent::BUTTON_PRESSED_DOWN:
                updateState(ElevatorState::MOVING_DOWN);
                break;
            case ElevatorEvent::STOP_BUTTON_PRESSED:
                updateState(ElevatorState::STOP);
                break;
            
            default:
                break;
            }
        }
        void handleEventMovingUp(ElevatorEvent event) {
            switch (event)
            {
            case ElevatorEvent::BUTTON_PRESSED_UP:
                updateState(ElevatorState::MOVING_UP);
                break;
            case ElevatorEvent::BUTTON_PRESSED_DOWN:
                updateState(ElevatorState::MOVING_DOWN);
                break;
            case ElevatorEvent::STOP_BUTTON_PRESSED:
                updateState(ElevatorState::STOP);
                break;
            
            default:
                break;
            }
        }
        void handleEventStop(ElevatorEvent event) {
            switch (event)
            {
            case ElevatorEvent::BUTTON_PRESSED_UP:
                updateState(ElevatorState::MOVING_UP);
                break;
            case ElevatorEvent::BUTTON_PRESSED_DOWN:
                updateState(ElevatorState::MOVING_DOWN);
                break;
            case ElevatorEvent::STOP_BUTTON_PRESSED:
                updateState(ElevatorState::STOP);
                break;
            
            default:
                break;
            }
        }
};