#include "inc/elevator_fsm.h"

Elevator::Elevator(ElevatorDriver* driver, ElevatorState initialState, uint8_t currentFloor)
    : driver(driver), currentState(initialState), currentFloor(currentFloor){
        stopButton = false;
        obstruction = false;
    }    

Elevator::~Elevator() {
    delete driver;
}
    
void Elevator::updateState(ElevatorState nextState) {

    currentState = nextState;

    entryState(currentState);
}

void Elevator::entryState(ElevatorState state) {
    switch (state)
    {
        case ElevatorState::INIT:
            
            if (!driver->get_connected()) {
                driver->connect(); 
            }
            
            // stop button poller
            // obstruction poller
            // floor sensor poller 
            // find valid state/floor?
            break;

        case ElevatorState::IDLE:
            driver->set_stop_lamp(0);


            // open door with timer
            // close door if no obstruction
            break;

        case ElevatorState::MOVING_UP:
            driver->set_motor_direction(1);
            driver->set_stop_lamp(0);
            // elevator direction up
            break;

        case ElevatorState::MOVING_DOWN:
            driver->set_motor_direction(-1);
            driver->set_stop_lamp(0);

            // elevator direction down
            break;

        case ElevatorState::STOP:
            driver->set_motor_direction(0);
            driver->set_stop_lamp(1);
            // if valid floor open door with timer
            break;

        default:
            break;
    }
}

void Elevator::handleEvent(ElevatorEvent event) {

    std::lock_guard<std::mutex> guard(eventMutex);

    switch(currentState) {
        
        // If elevator is in INIT it does not handle any event
        case ElevatorState::INIT:
            break;

        // Will handle orders and stop buttons
        case ElevatorState::IDLE:
            if (event == ElevatorEvent::STOP_BUTTON_PRESSED){
                updateState(ElevatorState::STOP);
            }
            else if (event == ElevatorEvent::ORDER_RECEIVED) {
                // Check which floor is above or below
                updateState(ElevatorState::MOVING_UP);
            }
            break;


        //What to do when the elevator is moving up
        case ElevatorState::MOVING_UP:
            if (event == ElevatorEvent::STOP_BUTTON_PRESSED){
                updateState(ElevatorState::STOP);
            }
            else if (event == ElevatorEvent::ARRIVED_AT_FLOOR) {
                if (shouldStop())
                {
                    updateState(ElevatorState::IDLE);
                }
                else
                {
                    updateState(ElevatorState::MOVING_UP); // can maybe remove?
                }
            }
            break;


        //What to do when the elevator is moving down
        case ElevatorState::MOVING_DOWN:
            if (event == ElevatorEvent::STOP_BUTTON_PRESSED){
                updateState(ElevatorState::STOP);
            }
            else if (event == ElevatorEvent::ARRIVED_AT_FLOOR) {
                if (shouldStop())
                {
                    updateState(ElevatorState::IDLE);
                }
                else
                {
                    updateState(ElevatorState::MOVING_DOWN); // can maybe remove?
                }
            }
            break;


        //What to do when the elevator is stopped
        case ElevatorState::STOP:
            if (event == ElevatorEvent::STOP_BUTTON_PRESSED){
                updateState(ElevatorState::STOP);
            }
            break;
    }
}

// Floor
void Elevator::setFloor(int8_t floor) {
    currentFloor = floor;
}

int8_t Elevator::getFloor() {
    return currentFloor;
}

void Elevator::floorPoller() {
    setFloor(driver->get_floor_sensor_signal());
}

// Stop button
void Elevator::pollStopButton() {
    stopButton = driver->get_stop_signal();
}

void Elevator::setStopButtonLight(uint8_t val) {
    driver->set_stop_lamp(val);
}

// Obstruction
void Elevator::pollObstruction() {
    obstruction = driver->get_obstruction_signal();
}


////////////////// EVENTS ////////////////////

//returns TRUE if elevator should stop at the floor it arrived at
//returns FALSE if the elevator should keep moving/idling
bool Elevator::shouldStop() {
    switch (Elevator::currentState)
    {
    case ElevatorState::MOVING_UP:
        return true; // temp true
            // e.requests[e.floor][CallType.cab]       || // is there a cab call at this floor              STOP
            // e.requests[e.floor][CallType.hallUp]    || // is there a hall call upwards at this floor     STOP
            // !e.requestsAbove                        || // there are no more requests above               STOP
            // e.floor == 0                            || ?? bug catching
            // e.floor == e.requests.length-1;            ?? idk
    case ElevatorState::MOVING_DOWN:
        return true; // temp true
            // e.requests[e.floor][CallType.cab]       || // is there a cab call at this floor              STOP
            // e.requests[e.floor][CallType.hallDown]  || // is there a hall call downwards at this floor   STOP
            // !e.requestsBelow                        || // there are no more requests below               STOP
            // e.floor == 0                            || //bug catching?
            // e.floor == e.requests.length-1;            //idk
    case ElevatorState::STOP:
        return true;
    }
}

