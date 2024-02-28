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

    std::vector<std::vector<bool>> button_vec =  std::vector<std::vector<bool>>(3, std::vector<bool>(number_of_floors, false));

    void poll_buttons() {
        for (int floor = 0; floor < number_of_floors; ++floor) {

            for (int button = 0; button < 3; ++button) {

                bool btn_status = driver->get_button_signal(button, floor);
                if ( btn_status != button_vec[button][floor]) {
                    
                    if (btn_status) {
                        button_vec[button][floor] = true;

                        button_type call_type = (button_type)button; //might give wrong button check
                        data_container->add_new_call_with_elevatorId(floor, call_type, elevator_id); 

                        printf("Button %d on floor %d pressed\n", button, floor);
                    } else {
                        button_vec[button][floor] = false;
                        printf("Button %d on floor %d released\n", button, floor);
                    }
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