#pragma once

#include "call_class.h"
#include "elevator_driver.h"
#include <boost/thread.hpp>

class button_poller {
private:
    elevator_driver* driver;
    Call_database* call_database;
    boost::thread poller_thread;
    bool running;
    int number_of_floors;

    void poll_buttons() {
        while (running) {
            for (int floor = 0; floor < number_of_floors; ++floor) {
                for (int button = 0; button < 3; ++button) {
                    if (driver->get_button_signal(button, floor) == 1) {

                        button_type call_type = (button_type)button;
                        call_database->add_call(floor, call_type); //TODO ADD REAL CALL FUNC
                    }
                }
            }
        }
    }

public:
    button_poller(elevator_driver* driver) : driver(driver), running(false) {}

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