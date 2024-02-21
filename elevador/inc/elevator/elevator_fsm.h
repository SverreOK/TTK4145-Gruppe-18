#pragma once
#ifndef ELEVATOR_INC_ELEVATOR_ELEVATOR_FSM_H
#define ELEVATOR_INC_ELEVATOR_ELEVATOR_FSM_H

//Includes
#include "elevator_driver.h" 
#include "elevator_algorithm.h"

#include "super_container.h"

// #include "event_queue.h" this does not exist

#include <vector>
#include <mutex>

// Forward declaration
class elevator_driver;

// EVENTS
enum class elevator_event {
    ORDER_RECEIVED,
    ARRIVED_AT_FLOOR,
    DOOR_TIMEOUT
};


class Elevator {
    private:
        elevator_driver* driver;

        std::mutex event_mutex;

        Super_container* data_container;

        Elevator_id id;

        void initialize_position();

    public:
        // Constructor declaration
        Elevator(elevator_driver* driver, Elevator_id id, Super_container* data_container);

        void handle_event(elevator_event event);

        // Door
        void open_door();
        void close_door();

        void run();

        ~Elevator();	

};

#endif ELEVATOR_INC_ELEVATOR_ELEVATOR_FSM_H