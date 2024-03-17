#include "elevator_button_poller.h"

Elevator_driver_poller::Elevator_driver_poller(elevator_driver* driver, Elevator_id elevator_id, Super_container* data_container, int number_of_floors, thread_safe_queue* event_queue, Elevator* elevator_fsm) 
  : driver(driver), 
    data_container(data_container), 
    elevator_id(elevator_id), 
    event_queue(event_queue), 
    elevator_fsm(elevator_fsm), 
    number_of_floors(number_of_floors),
    running(false);

void Elevator_driver_poller::poll_buttons() {
    for (int floor = 0; floor < number_of_floors; ++floor) {

        for (int button = 0; button < 3; ++button) {

            bool btn_status = driver->get_button_signal(button, floor);
            if ( btn_status != button_vec[button][floor]) {
                
                if (btn_status) {
                    button_vec[button][floor] = true;

                    button_type call_type = (button_type)button;
                    data_container->add_new_call_with_elevatorId(floor, call_type, elevator_id); 

                    //printf("Button %d on floor %d pressed\n", button, floor);
                } else {
                    button_vec[button][floor] = false;
                    //printf("Button %d on floor %d released\n", button, floor);
                }
            }
        }
    }
}

void Elevator_driver_poller::poll_floor_sensors() {
    
    int floor_sensor_signal = driver->get_floor_sensor_signal();

    if (floor_sensor_signal != -1 && floor_sensor_signal != data_container->get_elevator_by_id(elevator_id)->get_current_floor()) {
        
        data_container->get_elevator_by_id(elevator_id)->set_current_floor(floor_sensor_signal);
        event_queue->push(elevator_event::ARRIVED_AT_FLOOR);
    }
}

void Elevator_driver_poller::poll_obstruction() {
    bool obstruction_signal = driver->get_obstruction_signal();
    
    data_container->get_elevator_by_id(elevator_id)->set_obstruction(obstruction_signal);

    if (obstruction_signal) {
        elevator_fsm->on_obstruction_detected();
    } 
}

void Elevator_driver_poller::poll_all() {
    running = true;
    while (running) {
        poll_buttons();
        poll_floor_sensors();
        poll_obstruction();
        boost::this_thread::sleep_for(boost::chrono::milliseconds(POLLER_RATE_MS));
    }
}


void Elevator_driver_poller::stop() {
    running = false;
}

bool Elevator_driver_poller::get_running() {
    return running;
}