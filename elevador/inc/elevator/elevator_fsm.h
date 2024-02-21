#pragma once
#ifndef ELEVATOR_INC_ELEVATOR_ELEVATOR_FSM_H
#define ELEVATOR_INC_ELEVATOR_ELEVATOR_FSM_H

//Includes
#include "elevator_driver.h" // Include the correct header file for elevator_driver
#include "elevator_algorithm.h"
#include "inc/call_class.h"
#include <vector>
#include <mutex>

// EVENTS
enum class elevator_event {
    ORDER_RECEIVED,
    ARRIVED_AT_FLOOR,
    DOOR_TIMEOUT
};

// STATES
enum class state_enum {
    IDLE,
    MOVING_UP,
    MOVING_DOWN,
    DOOR_OPEN
};

struct elevator_state {
    state_enum current_state;
    int current_floor;
    bool obstruction = false;
    std::vector<int8_t,int8_t> orders;
};


class Elevator {
    private:
        elevator_driver* driver;
        elevator_state* state;

        std::mutex event_mutex;

        void initialize_position();

    public:
        // Constructor declaration
        Elevator(elevator_driver* driver, elevator_state* state);

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