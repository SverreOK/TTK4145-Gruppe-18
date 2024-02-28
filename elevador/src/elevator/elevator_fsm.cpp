#include "elevator/elevator_fsm.h"

Elevator::Elevator(elevator_driver* driver, Elevator_id id, Super_container* data_container, thread_safe_queue<elevator_event>& event_queue)
    : driver(driver), id(id), data_container(data_container), event_queue(event_queue){
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
        - where implement event queue?
        */

       /*
       EVENT QUEUE:
        What pushes the order received events?
        Door_timer pushes the order receieved event
        Poller pushes the arrived at floor event
        The class/object that runs the event queue can send handle event to the elevator
       */

    // ORDER RECEIVED
    case elevator_event::ORDER_RECEIVED:
        switch (current_state) {
            case state_enum::IDLE:
                std::cout << "Order received in IDLE " << std::endl;
                driver->set_motor_direction(motor_dir);
                if (motor_dir == 0) {
                    open_door();
                }
                else {
                    if (motor_dir == 1) {
                        // update state to moving up
                        data_container->get_elevator_by_id(id)->set_current_state(state_enum::MOVING_UP);
                    }
                    else if (motor_dir == -1) {
                        // update state to moving down
                        data_container->get_elevator_by_id(id)->set_current_state(state_enum::MOVING_DOWN);
                    }
                }
            break;

            case state_enum::DOOR_OPEN:
                std::cout << "Order receieved in DOOR_OPEN" << std::endl;
                if (1) { // if current_floor = new order floor
                    // reset door timer
                    // order complete at floor.
                    break;
                }
                break;

            case state_enum::MOVING_DOWN:
                std::cout << "Order received in MOVING_DOWN" << std::endl;
                break;
            case state_enum::MOVING_UP:
                std::cout << "Order received in MOVING_UP" << std::endl;
                break;
        }
        break;

    // ARRIVED AT FLOOR
    case elevator_event::ARRIVED_AT_FLOOR:
        std::cout << "Arrived at floor" << std::endl;
        if (1) {
            driver->set_motor_direction(0);
            open_door();
            // order complete at floor

        }    
        break;    

    // DOOR TIMEOUT
    case elevator_event::DOOR_TIMEOUT:
        std::cout << "Door timeout" << std::endl;

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
        data_container->get_elevator_by_id(id)->set_current_floor(driver->get_floor_sensor_signal());
        return;
    }

    driver->set_motor_direction(-1);
    while (driver->get_floor_sensor_signal() == -1) {
        // Wait for the elevator to reach the nearest floor
    }
    //update current floor
    data_container->get_elevator_by_id(id)->set_current_floor(driver->get_floor_sensor_signal());
    driver->set_motor_direction(0);
}

// This function starts the elevator
void Elevator::run_event_queue() {
    while (running) {
        elevator_event event = event_queue.pop();
        handle_event(event);
    }
}

void Elevator::start() {
    Elevator::initialize_position();
    running = true;
    fsm_thread = boost::thread(&Elevator::run_event_queue, this);
}

void Elevator::stop() {
    running = false;
        if (fsm_thread.joinable()) {
            fsm_thread.join();
        }
}

// Might cause linker errors
Elevator::~Elevator() {
    delete driver;
}