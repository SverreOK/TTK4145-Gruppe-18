#pragma once

#include "inc/data/call_class.h"
#include "inc/data/elevator_state_class.h"

class Super_container {
    private: 
        std::vector<Elevator_state*> elevators;
        std::shared_mutex mutex;

        std::vector <Call*> locally_assigned_calls;
        std::vector <Call*> call_list;
        std::shared_mutex call_list_mutex;

    public:
        std::vector<Elevator_id> get_alive_elevators();

        std::vector<Call*>get_call_list(); //Change this

        void add_call(int floor, button_type call_type, Call_id call_id);
        void add_call_with_elevatorId(int floor, button_type call_type, Elevator_id elevator_id);
        void change_call(Call* call, std::string elevator_id);

        void add_elevator(Elevator_state* elevator);

        Elevator_state* get_elevator_by_id(Elevator_id id);

        std::vector<Call*> get_calls_originating_from_elevator(Elevator_id elevator_id);
        Call_id* get_last_call_id_originating_from_elevator(Elevator_id elevator_id);

        std::vector<Call*> get_locally_assigned_calls();
};