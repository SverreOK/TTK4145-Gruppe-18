#pragma once

#include "call_class.h"
#include "event_queue.h"
#include "elevator_state_class.h"
#include <boost/thread/mutex.hpp>
#include <boost/chrono.hpp>

class Super_container {
    private: 
        std::vector<Elevator_state*> elevators;

        boost::mutex mtx;
        Elevator_id my_id;
        
        std::vector <Call*> locally_assigned_calls;
        std::vector <Call*> call_list;

        thread_safe_queue* event_queue;

    public:

        //constructor
        Super_container(thread_safe_queue* event_queue);

        std::vector<Elevator_id> get_alive_elevators();

        std::vector<Call*>get_call_list(); //Change this

        void add_new_call(int floor, button_type call_type, Call_id call_id);
        void add_new_call_with_elevatorId(int floor, button_type call_type, Elevator_id elevator_id);
        Call* add_call(Call* call); 

        void change_call(Call* call, std::string elevator_id);
        void push_new_call_event();
        int add_elevator(Elevator_state* elevator); //Return 1 if added, 0 if updated
        void remove_elevator(Elevator_id id);
        void set_my_id(Elevator_id id);


        void service_call(Call* call, Elevator_id elevator_id);

        bool similar_call_exists(button_type call_type, int floor, Elevator_id elevatpr_id);


        Elevator_state* get_elevator_by_id(Elevator_id id);
        Call_id* get_last_call_id_originating_from_elevator(Elevator_id elevator_id);
        std::vector<Call*> get_calls_originating_from_elevator(Elevator_id elevator_id);
        void update_locally_assigned_calls();
        std::vector<Call*> get_locally_assigned_calls();
        // std::vector<bool> get_local_cab_requests_cheeky();
        Elevator_id get_my_id();
};