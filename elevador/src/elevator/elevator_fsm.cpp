#include "elevator/elevator_fsm.h"

Elevator::Elevator(elevator_driver* driver, Elevator_id id, Super_container* data_container)
    : driver(driver), id(id), data_container(data_container) {
        driver->connect();        
}

void Elevator::handle_event(elevator_event event) {

    // Put algorithm variables here
    //int current_floor = data_container->get_elevator_by_id(id)->get_current_floor();
    //state_enum current_state = data_container->get_elevator_by_id(id)->get_current_state();
    //std::vector<Call*> call_list = data_container->get_locally_assigned_calls();

    switch (event)
    {
    // ORDER RECEIVED
    case elevator_event::ORDER_RECEIVED:
        break;

    // ARRIVED AT FLOOR
    case elevator_event::ARRIVED_AT_FLOOR:
        break;
        

    // DOOR TIMEOUT
    case elevator_event::DOOR_TIMEOUT:
        break;
    }
}

void Elevator::open_door() {
    // Open the door for 3 seconds
    driver->set_door_open_lamp(1);
    // start door timer
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
    // start the while loop
    // poll floor
    // poll obstruction
    // poll orders?
    // sleep for 10 ms
}

// Might cause linker errors
Elevator::~Elevator() {
    delete driver;
}