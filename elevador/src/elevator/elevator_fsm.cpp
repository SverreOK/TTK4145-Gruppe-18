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
    if (motor_dir != 0) {
        desired_direction = motor_dir;
    }

    std::cout << "DEBUG: Current floor: " << current_floor;
    std::cout << ", Call list size: " << call_list.size();
    std::cout << ", Motor dir: " << motor_dir << "-------------" << std::endl;
    

    switch (current_state)
    {
        case state_enum::IDLE:
            if(event == elevator_event::ORDER_RECEIVED || event == elevator_event::DOOR_TIMEOUT){
                std::cout << "Order received in IDLE " << std::endl;
                driver->set_motor_direction(motor_dir);

                if (motor_dir == 0 && event == elevator_event::ORDER_RECEIVED) {
                    clear_orders(call_list, current_floor, desired_direction);
                    open_door();
                }
                else if (motor_dir == 1) {
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
                    data_container->get_elevator_by_id(id)->set_current_state(state_enum::DOOR_OPEN);
                    // order complete at floor
                    clear_orders(call_list, current_floor, desired_direction);
                    open_door();
                }
            }

        case state_enum::DOOR_OPEN:
            if (event == elevator_event::DOOR_TIMEOUT){
                clear_orders(call_list, current_floor, desired_direction);
                driver->set_door_open_lamp(0);

                data_container->get_elevator_by_id(id)->set_current_state(state_enum::IDLE);
                event_queue->push(elevator_event::DOOR_TIMEOUT);
            }
    }
}


void Elevator::clear_orders(std::vector<Call*> call_list, int current_floor, int motor_dir) {
    for (auto call : call_list) {
        if ((call->get_floor() == current_floor && call->get_call_type() == button_type::CAB) ||
            (call->get_floor() == current_floor && call->get_call_type() == button_type::UP_HALL && motor_dir == 1) ||
            (call->get_floor() == current_floor && call->get_call_type() == button_type::DOWN_HALL && motor_dir == -1)){
        // if (call->get_floor() == current_floor) {
            data_container->service_call(call, id);
            std::cout << "Clearing calls at floor " << current_floor << std::endl;
        }
    }
}


void Elevator::open_door() {
    std::cout << "Door opened" << std::endl;
    driver->set_door_open_lamp(1);
    data_container->get_elevator_by_id(id)->set_current_state(state_enum::DOOR_OPEN);

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