#pragma once

#include "inc/call_class.h"
#include "elevator_driver.h"
#include <boost/thread.hpp>

class Elevator_driver_poller {
private:
    elevator_driver* driver;
    Call_database* call_database;
    boost::thread poller_thread;
    Elevator_id elevator_id;
    bool running;
    int number_of_floors;

    int current_floor;

    void poll_buttons() {
        for (int floor = 0; floor < number_of_floors; ++floor) {
            for (int button = 0; button < 3; ++button) {
                if (driver->get_button_signal(button, floor) == 1) {

                    button_type call_type = (button_type)button; //might give wrong button check
                    call_database->add_call_with_elevatorId(floor, call_type, elevator_id); 
                }
            }
        }
    }

    void poll_floor_sensors() {
        int floor = driver->get_floor_sensor_signal();
        if (floor != current_floor) {
            current_floor = floor;
            call_database->update_elevator_floor(elevator_id, floor);
        }
    }



public:
    Elevator_driver_poller(elevator_driver* driver, Elevator_id elevator_id, Call_database* call_database, int number_of_floors) 
        : driver(driver), running(false), elevator_id(elevator_id), call_database(call_database), number_of_floors(number_of_floors) {}

    void start() {
        running = true;
        poller_thread = boost::thread(&button_poller::poll_buttons, this);
    }

    void stop() {
        running = false;
        if (poller_thread.joinable()) {
            poller_thread.join();
        }
    }

    ~button_poller() {
        stop();
    }
};