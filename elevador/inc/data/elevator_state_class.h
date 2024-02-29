#pragma once

#include "call_class.h"
#include <boost/asio.hpp>
#include <boost/thread/mutex.hpp>


struct elevator_status_network
{
    char id[8];
    uint8_t state;
    int8_t floor;
    bool obstruction;
    bool alive;
};

class Elevator_state {
    private:
        Elevator_id id;
        state_enum current_state;
        int current_floor;
        bool obstruction;
        bool alive;
        boost::asio::ip::udp::endpoint endpoint;
        boost::mutex mtx;

    public:
        Elevator_state(Elevator_id id) : id(id){
            this->current_state = state_enum::IDLE;
            this->current_floor = 0;
            this->obstruction = false;
            this->alive = true;

        }

        state_enum get_current_state();
        int get_current_floor();
        bool get_obstruction_status();
        bool get_alive_status();
        elevator_status_network get_status_network();
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