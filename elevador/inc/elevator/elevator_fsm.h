#pragma once

//Includes
#include <boost/thread.hpp>

#include <vector>
#include <mutex>
#include <iostream>
#include <thread>


#include "elevator_driver.h" 
#include "elevator_algorithm.h"
#include "super_container.h"
#include "event_queue.h"

// // Forward declaration
// class elevator_driver;





class Elevator {
    private:
        elevator_driver* driver;

        std::mutex event_mutex;

        Super_container* data_container;

        Elevator_id id;

        thread_safe_queue* event_queue;

        boost::thread fsm_thread;

        int running;

        void initialize_position();

    public:
        // Constructor declaration
        Elevator(elevator_driver* driver, Elevator_id id, Super_container* data_container, thread_safe_queue* event_queue);

        void handle_event(elevator_event event);

        // Door
        void open_door();

        void run_event_queue();

        void start();
        void stop();

        ~Elevator();	

};