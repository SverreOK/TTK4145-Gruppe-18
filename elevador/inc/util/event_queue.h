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
    
    public: 
        // Pushes an element to the queue 
        void push(elevator_event item) 
        { 
            // Acquire lock 
            std::unique_lock<std::mutex> lock(m_mutex); 
    
            // Add item 
            m_queue.push(item); 
    
            // Notify one thread that 
            // is waiting 
            m_cond.notify_one(); 
        } 
    
        // Pops an element off the queue 
        /*
        To use, 
        Elevator::handle_event(event_queue.pop());    
        */
        elevator_event pop() 
        { 
    
            // acquire lock 
            std::unique_lock<std::mutex> lock(m_mutex); 
    
            // wait until queue is not empty 
            m_cond.wait(lock, 
                        [this]() { return !m_queue.empty(); }); 
    
            // retrieve item 
            elevator_event item = m_queue.front(); 
            m_queue.pop(); 
    
            // return item
            return item; 
        }

        bool empty() {
            std::unique_lock<std::mutex> lock(m_mutex);
            return m_queue.empty();
        }

        elevator_event front() {
            std::unique_lock<std::mutex> lock(m_mutex);
            return m_queue.front();
        }
}; 