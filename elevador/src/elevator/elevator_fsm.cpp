#include "elevator/elevator_fsm.h"

Elevator::Elevator(elevator_driver* driver, Elevator_id id, Super_container* data_container)
    : driver(driver), id(id), data_container(data_container) {
        driver->connect();        
}

void Elevator::handle_event(elevator_event event) {

    // Put algorithm variables here
    int current_floor = data_container->get_elevator_by_id(id)->get_current_floor();
    state_enum current_state = data_container->get_elevator_by_id(id)->get_current_state();
    std::vector<Call*> call_list = data_container->get_locally_assigned_calls();

    int motor_dir = choose_direction(current_floor, current_state, call_list);
    
    switch (event)
    {

        /* QUESTIONS:
        - GOOD: how do you update the state in super_container? data_container->get_elevator_by_id(id)->set_current_state();
        - how do you mark an order complete? 
        - how do i see the newest orders floor?
        */

    // ORDER RECEIVED
    case elevator_event::ORDER_RECEIVED:
        switch (current_state) {
            case state_enum::IDLE:
                driver->set_motor_direction(motor_dir);
                if (motor_dir = 0) {
                    open_door();
                }
                else {
                    if (motor_dir = 1) {
                        // update state to moving up
                        data_container->get_elevator_by_id(id)->set_current_state(state_enum::MOVING_UP);
                    }
                    else if (motor_dir = -1) {
                        // update state to moving down
                        data_container->get_elevator_by_id(id)->set_current_state(state_enum::MOVING_DOWN);
                    }
                }
            break;

            case state_enum::DOOR_OPEN:
                if (1) { // if current_floor = new order floor
                    // reset door timer
                    // order complete at floor.
                    break;
                }
                break;

            case state_enum::MOVING_DOWN:
                break;
            case state_enum::MOVING_UP:
                break;
        }
        break;

    // ARRIVED AT FLOOR
    case elevator_event::ARRIVED_AT_FLOOR:
        if (1) {
            driver->set_motor_direction(0);
            open_door();
            // order complete at floor

        }    
        break;    

    // DOOR TIMEOUT
    case elevator_event::DOOR_TIMEOUT:
        driver->set_door_open_lamp(0);
        int motor_dir = choose_direction(current_floor, current_state, call_list);
        driver->set_motor_direction(motor_dir);
        if (motor_dir == 0) {
            // set state to idle
            data_container->get_elevator_by_id(id)->set_current_state(state_enum::IDLE);
        }
        else {
            // set state to moving
            driver->set_motor_direction(motor_dir);
            if (motor_dir == 1) {
                data_container->get_elevator_by_id(id)->set_current_state(state_enum::MOVING_UP);
            }
            if (motor_dir == 0) {
                data_container->get_elevator_by_id(id)->set_current_state(state_enum::MOVING_DOWN);
            }
        }
        break;
    }
}

void Elevator::open_door() {
    // Open the door for 3 seconds
    driver->set_door_open_lamp(1);
    // start door timer
    // state change
    data_container->get_elevator_by_id(id)->set_current_state(state_enum::DOOR_OPEN);
}

void Elevator::close_door() {
    while(driver->get_obstruction_signal()) {
        // Wait for the obstruction to be cleared
    }
    driver->set_door_open_lamp(0);
}

// Initializes at closest floor near the bottom
void Elevator::initialize_position() {

    if (driver->get_floor_sensor_signal() != -1) {
        //update current floor
        return;
    }

    driver->set_motor_direction(-1);
    while (driver->get_floor_sensor_signal() == -1) {
        // Wait for the elevator to reach the nearest floor
    }
    //update current floor
    driver->set_motor_direction(0);
}

// This function starts the elevator
void Elevator::run() {
    Elevator::initialize_position();
    // start the while loop for the event queue?
    // sleep for 10 ms
}

// Might cause linker errors
Elevator::~Elevator() {
    delete driver;
}