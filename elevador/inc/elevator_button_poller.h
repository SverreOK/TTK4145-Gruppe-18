#pragma once

#include "call_class.h"
#include "elevator_driver.h"
#include <boost/thread.hpp>

class button_poller {
private:
    elevator_driver* elevator_driver_;
    boost::thread poller_thread_;
    bool running_;

    void poll_buttons() {
        while (running_) {
            for (int floor = 0; floor < elevator_driver_.get_number_of_floors(); ++floor) {
                for (int button = 0; button < 3; ++button) {
                    if (elevator_driver_.get_button_state(floor, button) == 1) {
                        elevator_driver_.create_call(floor, button);
                    }
                }
            }
        }
    }

public:
    button_poller(elevator_driver driver) : elevator_driver_(driver), running_(false) {}

    void start() {
        running_ = true;
        poller_thread_ = boost::thread(&button_poller::poll_buttons, this);
    }

    void stop() {
        running_ = false;
        if (poller_thread_.joinable()) {
            poller_thread_.join();
        }
    }

    ~button_poller() {
        stop();
    }
};