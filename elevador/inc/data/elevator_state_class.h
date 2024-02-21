#pragma once

#include "inc/data/call_class.h"

struct Elevator_id {
    std::string id;
};

class Elevator_state {
    private:
        static Elevator_id id;
        state_enum current_state;
        int current_floor;
        bool obstruction;
        bool alive;

        std::shared_mutex mutex;

    public:
        Elevator_state(Elevator_id id){
            id = id;
            current_state = state_enum::IDLE;
            current_floor = 0;
            obstruction = false;
            alive = true;

        }

        state_enum get_current_state();
        int get_current_floor();
        bool get_obstruction_status();
        bool get_alive_status();
        Elevator_id get_id();
        

        void set_current_state(state_enum state);
        void set_current_floor(int floor);
        void set_obstruction(bool obstruction);
        void set_alive(bool alive);


        // Needed for json creation:
        std::string get_behaviour();
        std::string get_direction();
        std::vector<bool> get_cab_requests();
};