#include "inc/elevator_fsm.h"

Elevator::Elevator(elevator_driver* driver, elevator_state initial_state, uint8_t current_floor)
    : driver(driver), current_state(initial_state), current_floor(current_floor){
        stop_button = false;
        obstruction = false;
    }    

Elevator::~Elevator() {
    delete driver;
}
    
void Elevator::update_state(elevator_state next_state) {

    current_state = next_state;

    entry_state(current_state);
}

void Elevator::entry_state(elevator_state state) {
    switch (state)
    {
        case elevator_state::INIT:
            
            if (!driver->get_connected()) {
                driver->connect(); 
            }
            
            // stop button poller
            // obstruction poller
            // floor sensor poller 
            // find valid state/floor?
            break;

        case elevator_state::IDLE:
            driver->set_stop_lamp(0);


            // open door with timer
            // close door if no obstruction
            break;

        case elevator_state::MOVING_UP:
            driver->set_motor_direction(1);
            driver->set_stop_lamp(0);
            // elevator direction up
            break;

        case elevator_state::MOVING_DOWN:
            driver->set_motor_direction(-1);
            driver->set_stop_lamp(0);

            // elevator direction down
            break;

        case elevator_state::STOP:
            driver->set_motor_direction(0);
            driver->set_stop_lamp(1);
            // if valid floor open door with timer
            break;

        default:
            break;
    }
}

void Elevator::handle_event(elevator_event event) {

    std::lock_guard<std::mutex> guard(event_mutex);

    switch(current_state) {
        
        // If elevator is in INIT it does not handle any event
        case elevator_state::INIT:
            break;

        // Will handle orders and stop buttons
        case elevator_state::IDLE:
            if (event == elevator_event::STOP_BUTTON_PRESSED){
                update_state(elevator_state::STOP);
            }
            else if (event == elevator_event::ORDER_RECEIVED) {
                // Check which floor is above or below
                update_state(elevator_state::MOVING_UP);
            }
            break;


        //What to do when the elevator is moving up
        case elevator_state::MOVING_UP:
            if (event == elevator_event::STOP_BUTTON_PRESSED){
                update_state(elevator_state::STOP);
            }
            else if (event == elevator_event::ARRIVED_AT_FLOOR) {
                if (should_stop())
                {
                    update_state(elevator_state::IDLE);
                }
                else
                {
                    update_state(elevator_state::MOVING_UP); // can maybe remove?
                }
            }
            break;


        //What to do when the elevator is moving down
        case elevator_state::MOVING_DOWN:
            if (event == elevator_event::STOP_BUTTON_PRESSED){
                update_state(elevator_state::STOP);
            }
            else if (event == elevator_event::ARRIVED_AT_FLOOR) {
                if (should_stop())
                {
                    update_state(elevator_state::IDLE);
                }
                else
                {
                    update_state(elevator_state::MOVING_DOWN); // can maybe remove?
                }
            }
            break;


        //What to do when the elevator is stopped
        case elevator_state::STOP:
            if (event == elevator_event::STOP_BUTTON_PRESSED){
                update_state(elevator_state::STOP);
            }
            break;
    }
}

// Floor
void Elevator::set_floor(int8_t floor) {
    current_floor = floor;
}

int8_t Elevator::get_floor() {
    return current_floor;
}

void Elevator::floor_poller() {
    set_floor(driver->get_floor_sensor_signal());
}

// Stop button
void Elevator::poll_stop_button() {
    stop_button = driver->get_stop_signal();
}

void Elevator::set_stop_button_light(uint8_t val) {
    driver->set_stop_lamp(val);
}

// Obstruction
void Elevator::poll_obstruction() {
    obstruction = driver->get_obstruction_signal();
}


////////////////// EVENTS ////////////////////

//returns TRUE if elevator should stop at the floor it arrived at
//returns FALSE if the elevator should keep moving/idling
bool Elevator::should_stop() {
    switch (Elevator::current_state)
    {
    case elevator_state::MOVING_UP:
        return true; // temp true
            // e.requests[e.floor][CallType.cab]       || // is there a cab call at this floor              STOP
            // e.requests[e.floor][CallType.hallUp]    || // is there a hall call upwards at this floor     STOP
            // !e.requestsAbove                        || // there are no more requests above               STOP
            // e.floor == 0                            || ?? bug catching
            // e.floor == e.requests.length-1;            ?? idk
    case elevator_state::MOVING_DOWN:
        return true; // temp true
            // e.requests[e.floor][CallType.cab]       || // is there a cab call at this floor              STOP
            // e.requests[e.floor][CallType.hallDown]  || // is there a hall call downwards at this floor   STOP
            // !e.requestsBelow                        || // there are no more requests below               STOP
            // e.floor == 0                            || //bug catching?
            // e.floor == e.requests.length-1;            //idk
    case elevator_state::STOP:
        return true;
    }
}

