#pragma once
#ifndef ELEVATOR_INC_ELEVATOR_ELEVATOR_FSM_H
#define ELEVATOR_INC_ELEVATOR_ELEVATOR_FSM_H

//Includes
#include "inc/elevator/elevator_driver.h" 
#include "inc/elevator/elevator_algorithm.h"

#include "inc/data/super_container.h"

#include <vector>
#include <mutex>

// EVENTS
enum class elevator_event {
    ORDER_RECEIVED,
    ARRIVED_AT_FLOOR,
    DOOR_TIMEOUT
};


class Elevator {
    private:
        elevator_driver* driver;
        Elevator_state* state;

        std::mutex event_mutex;

        void initialize_position();

    public:
        // Constructor declaration
        Elevator(elevator_driver* driver, Elevator_state* state);

        void handle_event(elevator_event event);

        // Floor
        int8_t get_floor();
        void set_floor(int8_t floor);

        // Door
        void open_door();
        void close_door();
        bool is_door_open();

        void run();

        ~Elevator();	

};

#endif ELEVATOR_INC_ELEVATOR_ELEVATOR_FSM_H