#pragma once


#include "super_container.h"

#include "elevator_driver.h"

#include <boost/thread.hpp>

#include "elevator_fsm.h"

class Elevator_driver_poller {

private:
    elevator_driver* driver;
    Super_container* data_container;
    Elevator_id elevator_id;
    thread_safe_queue* event_queue;
    Elevator* elevator_fsm;

    bool running;
    int number_of_floors;
    int current_floor;

    std::vector<std::vector<bool>> button_vec =  std::vector<std::vector<bool>>(3, std::vector<bool>(number_of_floors, false));

    void poll_buttons();

    void poll_floor_sensors();

    void poll_obstruction();

public:

    void poll_all();

    Elevator_driver_poller(elevator_driver* driver, Elevator_id elevator_id, Super_container* data_container, int number_of_floors, thread_safe_queue* event_queue, Elevator* elevator_fsm) 
        : driver(driver), data_container(data_container), elevator_id(elevator_id), event_queue(event_queue), elevator_fsm(elevator_fsm), number_of_floors(number_of_floors) {
            running = false;
        }


    void stop();

    bool get_running();

    ~Elevator_driver_poller() {
        stop();
    }
};