#pragma once
#ifndef ELEVATOR_INC_ELEVATOR_ELEVATOR_FSM_H
#define ELEVATOR_INC_ELEVATOR_ELEVATOR_FSM_H

//Includes
#include "elevator_driver.h" // Include the correct header file for elevator_driver
#include "call_class.h"
#include <vector>
#include <mutex>

// STATES
enum class state {
    INIT,
    IDLE,
    MOVING_UP,
    MOVING_DOWN,
    STOP,
    DOOR_OPEN
};

struct elevator_state {
    state current_state;
    int current_floor;
    bool door_open = false;
    bool obstruction = false;
    std::vector<int8_t,int8_t> orders;
};


class Elevator {
    private:
        elevator_driver* driver;
        elevator_state* state;

        std::mutex event_mutex;

    public:
        void update_elevator_state();
        void entry_state();
        void handle_event();
        void event_poller();

        // Floor
        void poll_floor();
        int8_t get_floor();
        void set_floor(int8_t floor);

        // Stop button
        void poll_stop_button();
        bool is_stop_button_pressed();

        // Door
        void open_door();
        bool is_door_open();

        Elevator(elevator_driver* driver, elevator_state* state);
        ~Elevator();	

};

#endif ELEVATOR_INC_ELEVATOR_ELEVATOR_FSM_H