#include "event_queue.h"

// Pushes an element to the queue 
void thread_safe_queue::push(elevator_event item) { 
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
elevator_event thread_safe_queue::pop() { 

    // acquire lock 
    std::unique_lock<std::mutex> lock(m_mutex); 

    // wait until queue is not empty 
    m_cond.wait(lock, 
                [this]() { return !m_queue.empty(); }); 

    // retrieve item 
    elevator_event item = m_queue.front();
    m_queue.pop(); 
    last_popped_event = item;

    // return item
    return item; 
}

bool thread_safe_queue::empty() {
    std::unique_lock<std::mutex> lock(m_mutex);
    return m_queue.empty();
}

elevator_event thread_safe_queue::front() {
    std::unique_lock<std::mutex> lock(m_mutex);
    return m_queue.front();
}

elevator_event thread_safe_queue::get_last_popped_event() {
    std::lock_guard<std::mutex> lock(m_mutex);
    return last_popped_event;
}