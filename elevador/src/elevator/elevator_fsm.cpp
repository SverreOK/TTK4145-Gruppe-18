#include "inc/elevator/elevator_fsm.h"

Elevator::Elevator(elevator_driver* driver, elevator_state* state)
    : driver(driver), state(state), door_timer(){
        driver->connect();
        state->current_state = state_enum::IDLE;
        state->current_floor = -1;
        state->obstruction = driver->get_obstruction_signal();
        
}

void Elevator::handle_event(elevator_event event) {
    switch (event)
    {
    // ORDER RECEIVED
    case elevator_event::ORDER_RECEIVED:
        // ANDERS MAKE THIS FUNCTION
        break;

    // ARRIVED AT FLOOR
    case elevator_event::ARRIVED_AT_FLOOR:
        if (should_stop(this, *state)) {
            driver->set_motor_direction(0);
            state->current_state = state_enum::DOOR_OPEN;
            open_door();

            // order at this floor is done and can be cleared
        }

    // DOOR TIMEOUT
    case elevator_event::DOOR_TIMEOUT:
        close_door();
        if (choose_direction(this, *state) == 1) {
            state->current_state = state_enum::MOVING_UP;
            driver->set_motor_direction(1);
        } else if (choose_direction(this, *state) == -1) {
            state->current_state = state_enum::MOVING_DOWN;
            driver->set_motor_direction(-1);
        } else {
            state->current_state = state_enum::IDLE;
            driver->set_motor_direction(0);
        }
    }
}

int8_t Elevator::get_floor() {
    return state->current_floor;
}

void Elevator::set_floor(int8_t floor) {
    state->current_floor = floor;
}

void Elevator::open_door() {
    // Open the door for 3 seconds
    driver->set_door_open_lamp(1);
    door_timer.start(handle_event(elevator_event::DOOR_TIMEOUT));
}

void Elevator::close_door() {
    while(driver->get_obstruction_signal()) {
        // Wait for the obstruction to be cleared
    }
    driver->set_door_open_lamp(0);
}

bool Elevator::is_door_open() {
    return driver->get_obstruction_signal();
}

// Initializes at closest floor near the bottom
void Elevator::initialize_position() {

    if (driver->get_floor_sensor_signal() != -1) {
        state->current_floor = driver->get_floor_sensor_signal();
        return;
    }

    driver->set_motor_direction(-1);
    while (driver->get_floor_sensor_signal() == -1) {
        // Wait for the elevator to reach the nearest floor
    }
    state->current_floor = driver->get_floor_sensor_signal();
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

Elevator::~Elevator() {
    delete driver;
}