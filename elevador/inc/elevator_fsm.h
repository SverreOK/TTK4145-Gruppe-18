#pragma once
#ifndef ELEVATOR_FSM_H
#define ELEVATOR_FSM_H
#define BOOST_THREAD_USE_LIB
// INCLUDES
#include "elevator_driver.h"
#include "call-class.h"
#include <vector>
#include <mutex>

// EVENTS
enum class elevator_event {
    ORDER_RECEIVED,
    STOP_BUTTON_PRESSED,
    ARRIVED_AT_FLOOR,
    OBSTRUCTION
};

// STATES
enum class elevator_state {
    INIT,
    IDLE,
    MOVING_UP,
    MOVING_DOWN,
    STOP,
    DOOR_OPEN
};

class Elevator {
private:
    elevator_driver* driver;
    elevator_state current_state;
    int8_t current_floor;
    
    uint8_t stop_button;
    uint8_t obstruction;

    std::vector<int,int> requests;
    
    std::mutex event_mutex;
    
public:

    void update_state(elevator_state state);
    void entry_state(elevator_state state);
    void handle_event(elevator_event event);
    elevator_state get_state();

    // Floor
    void set_floor(int8_t floor);
    int8_t get_floor();
    void floor_poller();

    // Stop button
    void poll_stop_button();
    void set_stop_button_light(uint8_t);

    // Obstruction
    void poll_obstruction();

    //algorithms
    bool should_stop();


    Elevator(elevator_driver* driver, elevator_state initial_state, uint8_t current_floor);
    ~Elevator();
};

#endif /* ELEVATOR_FSM_H */