#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>

#include "elevator/elevator_fsm.h"

// Thread-safe queue 
class thread_safe_queue { 
private: 
    // Underlying queue 
    std::queue<state_enum> m_queue; 
  
    // mutex for thread synchronization 
    std::mutex m_mutex; 
  
    // Condition variable for signaling 
    std::condition_variable m_cond; 
  
public: 
    // Pushes an element to the queue 
    void push(state_enum item) 
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
    state_enum pop() 
    { 
  
        // acquire lock 
        std::unique_lock<std::mutex> lock(m_mutex); 
  
        // wait until queue is not empty 
        m_cond.wait(lock, 
                    [this]() { return !m_queue.empty(); }); 
  
        // retrieve item 
        state_enum item = m_queue.front(); 
        m_queue.pop(); 
  
        // return item
        return item; 
    } 
}; 