#pragma once


#include "super_container.h"

#include "elevator_driver.h"

#include <boost/thread.hpp>

class Elevator_driver_poller {
private:
    elevator_driver* driver;
    Super_container* data_container;
    Elevator_id elevator_id;

    bool running;
    int number_of_floors;
    int update_freq = 100; //hz
    int current_floor;

    void poll_buttons() {
        for (int floor = 0; floor < number_of_floors; ++floor) {
            for (int button = 0; button < 3; ++button) {
                if (driver->get_button_signal(button, floor) == 1) {

                    button_type call_type = (button_type)button; //might give wrong button check
                    data_container->add_call_with_elevatorId(floor, call_type, elevator_id); 
                }
            }
        }
    }


public:

    void poll_all() {
        running = true;
        while (running) {
            poll_buttons();
            // poll_floor_sensors();
            boost::this_thread::sleep_for(boost::chrono::milliseconds(int(1/(update_freq*1000))));
        }
    }

    Elevator_driver_poller(elevator_driver* driver, Elevator_id elevator_id, Super_container* data_container, int number_of_floors) 
        : driver(driver), running(false), elevator_id(elevator_id), data_container(data_container), number_of_floors(number_of_floors) {}


    void stop() {
        running = false;
    }

    bool get_running() {
        return running;
    }

    ~Elevator_driver_poller() {
        stop();
    }
};