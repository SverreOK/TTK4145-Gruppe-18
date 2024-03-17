#include "elevator/elevator_fsm.h"

Elevator::Elevator(elevator_driver* driver, Elevator_id id, Super_container* data_container, thread_safe_queue* event_queue)
    : driver(driver), id(id), data_container(data_container), event_queue(event_queue){
       
        Current_direction = 0;
}

void Elevator::handle_event(elevator_event event) {

    // Put algorithm variables here
    int current_floor = data_container->get_elevator_by_id(id)->get_current_floor();
    state_enum current_state = data_container->get_elevator_by_id(id)->get_current_state();
    std::vector<Call*> call_list = data_container->get_locally_assigned_calls();
    //int motor_dir = choose_direction(current_floor, current_state, call_list);

    //main FSM logic:
    switch (current_state){

        case state_enum::MOVING_UP:
        case state_enum::MOVING_DOWN:
            if(event == elevator_event::ARRIVED_AT_FLOOR){
                driver -> set_floor_indicator(current_floor);
                
                if (should_stop(current_floor, Current_direction, call_list)){
                    driver->set_motor_direction(0);
                    data_container->get_elevator_by_id(id)->set_current_state(state_enum::DOOR_OPEN);
                    open_door(); //NOTE: this might need to be moved to after the clear_orders function
                    clear_orders(call_list, current_floor, Current_direction);
                }
            }
        break;

        case state_enum::IDLE:

            //only trigger if new call is received or door closes?

            if (event == elevator_event::ORDER_RECEIVED ||
                event == elevator_event::DOOR_CLOSED){
                    int motor_dir = choose_direction(current_floor, Current_direction, call_list);
                    Current_direction = motor_dir;
                    driver->set_motor_direction(motor_dir);
                    if (Current_direction == 0){
                        if(requests_same_floor(current_floor, call_list)){
                            data_container->get_elevator_by_id(id)->set_current_state(state_enum::DOOR_OPEN);
                            open_door(); //NOTE: this might need to be moved to after the clear_orders function
                            clear_orders(call_list, current_floor, Current_direction);
                        }
                    }
                    if (Current_direction != 0){ //moving!
                        if (Current_direction == 1){
                        data_container->get_elevator_by_id(id)->set_current_state(state_enum::MOVING_UP);
                        }
                        else if (Current_direction == -1){
                            data_container->get_elevator_by_id(id)->set_current_state(state_enum::MOVING_DOWN);
                        }
                    }

                }
            else {
                //std::cout << "wierd event while in IDLE" << std::endl;
            }


        break;

        case state_enum::DOOR_OPEN:

            if(event == elevator_event::ORDER_RECEIVED){
                clear_orders(call_list, current_floor, Current_direction);
            }

            else if(event == elevator_event::DOOR_TIMEOUT){
                //close door light
                data_container->get_elevator_by_id(id)->set_current_state(state_enum::IDLE);
                event_queue->push(elevator_event::DOOR_CLOSED);
                driver->set_door_open_lamp(0);
            }
        break;
    }
}

void Elevator::clear_orders(std::vector<Call*> call_list, int current_floor, int current_direction) {
    
    bool more_calls_up = requests_above(current_floor, call_list);
    bool more_calls_down = requests_below(current_floor, call_list);
    
    for (auto call : call_list) {

        if (call->get_floor() == current_floor){

            button_type call_type = call->get_call_type();

            if (call_type == button_type::CAB){ //always clear cab call if its at the current floor
                data_container->service_call(call, id); 
            }

            switch (current_direction){
                case 1: //moving up
                    if (call_type == button_type::UP_HALL){ //and call is up at this floor
                        data_container->service_call(call, id);
                    }
                    else if(call_type == button_type::DOWN_HALL && !more_calls_up){ //or call is down at this floor and there are no more calls up
                        data_container->service_call(call, id);
                    }
                break;

                case -1: //moving down
                    if (call_type == button_type::DOWN_HALL){ //and call is down at this floor
                        data_container->service_call(call, id);
                    }
                    else if(call_type == button_type::UP_HALL && !more_calls_down){ //or call is up at this floor and there are no more calls down
                        data_container->service_call(call, id);
                    }
                break;

                case 0:
                    if (call_type == button_type::UP_HALL || call_type == button_type::DOWN_HALL){ //and call is up or down at this floor
                        data_container->service_call(call, id);
                    }
            }
        }
    }
}


void Elevator::open_door() {
    //std::cout << "Door opened" << std::endl;
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
            boost::this_thread::sleep_for(boost::chrono::milliseconds(DOOR_OPEN_TIME_MS));
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
        elevator_event event = event_queue->pop();
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