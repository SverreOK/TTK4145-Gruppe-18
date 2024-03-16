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
#include "config.h"

// // Forward declaration
// class elevator_driver;





class Elevator {
    private:
        elevator_driver* driver;

        Elevator_id id;

        Super_container* data_container;

        thread_safe_queue* event_queue;

        boost::thread fsm_thread;

        int Current_direction;
        
        state_enum last_move_state;

        boost::thread door_timer_thread;

        std::mutex event_mutex;


        int running;

        void initialize_position();

        void clear_orders(std::vector<Call*> call_list, int current_floor, int current_direction);

    public:
        Elevator(elevator_driver* driver, Elevator_id id, Super_container* data_container, thread_safe_queue* event_queue);

        void handle_event(elevator_event event);

        // Door
        void open_door();
        void door_timer();
        void on_obstruction_detected();

        void run_event_queue();

        void start();
        void stop();

        ~Elevator();	

};