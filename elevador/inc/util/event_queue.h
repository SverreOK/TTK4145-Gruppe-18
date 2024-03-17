#pragma once

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>

// #include "elevator_fsm.h"


// EVENTS
enum class elevator_event {
    ORDER_RECEIVED,
    ARRIVED_AT_FLOOR,
    DOOR_TIMEOUT,
    OBSTRUCTION_DETECTED,
    DOOR_CLOSED
};

// Thread-safe queue 
class thread_safe_queue { 
    private: 
        // Underlying queue 
        std::queue<elevator_event> m_queue; 
    
        // mutex for thread synchronization 
        std::mutex m_mutex; 
    
        // Condition variable for signaling 
        std::condition_variable m_cond; 

        elevator_event last_popped_event;
    
    public: 
        void push(elevator_event item);
        elevator_event pop();

        bool empty();

        elevator_event front();
        elevator_event get_last_popped_event();
}; 