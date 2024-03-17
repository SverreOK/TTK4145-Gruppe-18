#pragma once

#include <boost/thread.hpp>

#include "super_container.h"
#include "elevator_driver.h"
#include "elevator_fsm.h"

class Elevator_driver_poller {

    private:
        Elevator* elevator_fsm;
        Elevator_id elevator_id;
        elevator_driver* driver;
        Super_container* data_container;
        thread_safe_queue* event_queue;

        int number_of_floors;
        int current_floor;
        bool running;

        std::vector<std::vector<bool>> button_vec =  std::vector<std::vector<bool>>(3, std::vector<bool>(number_of_floors, false));

        void poll_buttons();
        void poll_floor_sensors();
        void poll_obstruction();

    public:
        Elevator_driver_poller(elevator_driver* driver, Elevator_id elevator_id, Super_container* data_container, int number_of_floors, thread_safe_queue* event_queue, Elevator* elevator_fsm);

        void poll_all();
        void stop();
        bool get_running();

        ~Elevator_driver_poller() {
            stop();
        }
};