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
    std::cout << ", Motor dir: " << motor_dir << "-------------";

    switch (current_state){
        case state_enum::IDLE:
            std::cout << "IDLE" << std::endl;
        break;
        case state_enum::MOVING_UP:
            std::cout << "MOVING_UP" << std::endl;
        break;
        case state_enum::MOVING_DOWN:
            std::cout << "MOVING_DOWN" << std::endl;
        break;
    }

    switch (event)
    {
    case elevator_event::DOOR_TIMEOUT:
        std::cout << "DOOR_TIMEOUT" << std::endl;
        break;
    }

    switch (current_state)
    {
        case state_enum::IDLE:
            if(event == elevator_event::ORDER_RECEIVED){
                std::cout << "Order received in IDLE " << std::endl;
                driver->set_motor_direction(motor_dir);

                if (motor_dir == 0 && event == elevator_event::ORDER_RECEIVED) {
                    data_container->get_elevator_by_id(id)->set_current_state(state_enum::DOOR_OPEN);
                    clear_orders(call_list, current_floor, current_state);
                    open_door();
                    data_container->get_elevator_by_id(id)->set_current_state(state_enum::IDLE);
                }
                else if (motor_dir == 1) {
                    data_container->get_elevator_by_id(id)->set_current_state(state_enum::MOVING_UP);
                }
                else if (motor_dir == -1) {
                    data_container->get_elevator_by_id(id)->set_current_state(state_enum::MOVING_DOWN);
                }
            }

            if (event == elevator_event::DOOR_TIMEOUT) {
                driver->set_motor_direction(motor_dir);
                if (motor_dir == 1) {
                    data_container->get_elevator_by_id(id)->set_current_state(state_enum::MOVING_UP);
                }
                else if (motor_dir == -1) {
                    data_container->get_elevator_by_id(id)->set_current_state(state_enum::MOVING_DOWN);
                }
            }
        break;

        case state_enum::MOVING_DOWN:
        case state_enum::MOVING_UP:

            if (event == elevator_event::ARRIVED_AT_FLOOR){
                if (should_stop(current_floor, current_state, call_list)) {
                    driver->set_motor_direction(0);

                    state_enum last_move_state = current_state;
                    data_container->get_elevator_by_id(id)->set_current_state(state_enum::DOOR_OPEN);
                    clear_orders(call_list, current_floor, current_state);
                    open_door();
                    data_container->get_elevator_by_id(id)->set_current_state(last_move_state);
                }
            }


            if (event == elevator_event::DOOR_TIMEOUT) {
                driver->set_motor_direction(motor_dir);
                if (motor_dir == 0){
                    data_container->get_elevator_by_id(id)->set_current_state(state_enum::IDLE);
                }
                else if (motor_dir == 1) {
                    data_container->get_elevator_by_id(id)->set_current_state(state_enum::MOVING_UP);
                }
                else if (motor_dir == -1) {
                    data_container->get_elevator_by_id(id)->set_current_state(state_enum::MOVING_DOWN);
                }

            }
        break;
    }
}


void Elevator::clear_orders(std::vector<Call*> call_list, int current_floor, state_enum state) {
    
    bool more_calls_up = requests_above(current_floor, call_list);
    bool more_calls_down = requests_below(current_floor, call_list);
    
    for (auto call : call_list) {
        if (call->get_floor() == current_floor){

            bool cab_case = call->get_call_type() == button_type::CAB;

            bool up_and_up_case     = call->get_call_type() == button_type::UP_HALL     && state == state_enum::MOVING_UP;
            bool down_and_down_case = call->get_call_type() == button_type::DOWN_HALL   && state == state_enum::MOVING_DOWN;

            bool up_and_no_more_calls   = call->get_call_type() == button_type::UP_HALL     && !more_calls_down;
            bool down_and_no_more_calls = call->get_call_type() == button_type::DOWN_HALL   && !more_calls_up;

            if ((cab_case) ||
                (up_and_up_case) ||
                (down_and_down_case) ||
                (up_and_no_more_calls) ||
                (down_and_no_more_calls)) {

            data_container->service_call(call, id);
            std::cout << "Clearing calls at floor " << current_floor << std::endl;
        }
        } 
    }
}


void Elevator::open_door() {
    std::cout << "Door opened" << std::endl;
    driver->set_door_open_lamp(1);
    // data_container->get_elevator_by_id(id)->set_current_state(state_enum::DOOR_OPEN);

    if (door_timer_thread.joinable()) {
        door_timer_thread.interrupt();
        door_timer_thread.join();
    }

    door_timer_thread = boost::thread(&Elevator::door_timer, this);
}

void Elevator::door_timer() {
    while (true) {
        try {
            boost::this_thread::sleep_for(boost::chrono::seconds(3));
            break; // If sleep completes without interruption, break loop
        } catch (boost::thread_interrupted&) {
            // If sleep is interrupted, continue loop to restart sleep
            continue;
        }
    }
    
    driver->set_door_open_lamp(0);
    event_queue->push(elevator_event::DOOR_TIMEOUT);
}

void Elevator::on_obstruction_detected() {
    if (door_timer_thread.joinable()) {
        door_timer_thread.interrupt();
    }
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