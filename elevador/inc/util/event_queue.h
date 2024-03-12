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
            switch (item)
            { 
                case elevator_event::ORDER_RECEIVED:
                    std::cout << "EVENT_QUEUE: Pushing order received event" << std::endl;
                    break;
                case elevator_event::ARRIVED_AT_FLOOR:
                    std::cout << "EVENT_QUEUE: Pushing arrived at floor event" << std::endl;
                    break;
                case elevator_event::DOOR_TIMEOUT:
                    std::cout << "EVENT_QUEUE: Pushing door timer expired event" << std::endl;
                    break;
            }

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
}; 