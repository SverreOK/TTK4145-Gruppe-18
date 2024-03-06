#include "elevator/elevator_fsm.h"

Elevator::Elevator(elevator_driver* driver, Elevator_id id, Super_container* data_container, thread_safe_queue* event_queue)
    : driver(driver), id(id), data_container(data_container), event_queue(event_queue){
        //driver->connect();        
}

void Elevator::handle_event(elevator_event event) {

    // Put algorithm variables here
    int current_floor = data_container->get_elevator_by_id(id)->get_current_floor();
    state_enum current_state = data_container->get_elevator_by_id(id)->get_current_state();
    std::vector<Call*> call_list = data_container->get_locally_assigned_calls();
    int motor_dir = choose_direction(current_floor, current_state, call_list);

    std::cout << "DEBUG: Current floor: " << current_floor;

    std::cout << ", Call list size: " << call_list.size();

    std::cout << ", Motor dir: " << motor_dir << "-------------" << std::endl;
    
    switch (event)
    {

    // ORDER RECEIVED
    case elevator_event::ORDER_RECEIVED:
        switch (current_state) {
            case state_enum::IDLE:
                std::cout << "Order received in IDLE " << std::endl;
                driver->set_motor_direction(motor_dir);
                if (motor_dir == 0) {
                    clear_orders(call_list, current_floor);
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
                // if condition should be to check the newest item in the call list and check if its floor is equal to current flor
                clear_orders(call_list, current_floor);
                open_door();                
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
        switch (current_state)
        {
            case state_enum::MOVING_DOWN:
                std::cout << "Arrived at floor in MOVING_DOWN" << std::endl;
                break;
            case state_enum::MOVING_UP:
                std::cout << "Arrived at floor in MOVING_UP" << std::endl;
                break;
            case state_enum::IDLE:
                std::cout << "Arrived at floor in IDLE" << std::endl;
                break;
        }

        if (should_stop(current_floor, current_state, call_list)) {
            driver->set_motor_direction(0);
            data_container->get_elevator_by_id(id)->set_current_state(state_enum::DOOR_OPEN);
            // order complete at floor
            clear_orders(call_list, current_floor);
            open_door();

        }    
        break;    

    // DOOR TIMEOUT
    case elevator_event::DOOR_TIMEOUT:
        std::cout << "Door timeout" << std::endl;

        clear_orders(call_list, current_floor);

        driver->set_door_open_lamp(0);
        int motor_dir = choose_direction(current_floor, state_enum::IDLE, call_list);
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
            if (motor_dir == -1) {
                data_container->get_elevator_by_id(id)->set_current_state(state_enum::MOVING_DOWN);
            }
        }
        break;
    }
}

void Elevator::clear_orders(std::vector<Call*> call_list, int current_floor) {
    for (auto call : call_list) {
        if (call->get_floor() == current_floor) {
            data_container->service_call(call, id);
            std::cout << "Clearing calls at floor " << current_floor << std::endl;
        }
    }
}

void Elevator::open_door() {
    // Open the door for 3 seconds
    std::cout << "Door opened" << std::endl;
    driver->set_door_open_lamp(1);
    // state change
    data_container->get_elevator_by_id(id)->set_current_state(state_enum::DOOR_OPEN);
    while (true) {
        if (!driver->get_obstruction_signal()) {
            std::this_thread::sleep_for(std::chrono::seconds(3)); // Door remains open for 3 seconds if not obstructed
            if (!driver->get_obstruction_signal()) { // Check again to avoid
                break;
            }
        }
    }
    // Door timeout should be handled right after??
    handle_event(elevator_event::DOOR_TIMEOUT);
}

// Initializes at closest floor near the bottom
void Elevator::initialize_position() {

    if (driver->get_floor_sensor_signal() != -1) {
        //update current floor
        std::cout << "Elevator already at floor" << driver->get_floor_sensor_signal() << std::endl;
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
    // Implement in main instead
    while (running) {
        if (!event_queue->empty()) {
            elevator_event event = event_queue->pop();
            handle_event(event);
        }
        boost::this_thread::sleep_for(boost::chrono::milliseconds(100));
        //open_door();
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