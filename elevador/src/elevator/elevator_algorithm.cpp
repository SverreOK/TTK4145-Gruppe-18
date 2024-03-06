#pragma once

#include "elevator_algorithm.h"
#include <iostream>

// N_FLOORS IS DEFINED IN DRIVERS
// button_type IS DEFINED IN CALL_CLASS

bool requests_above(int current_floor, std::vector<Call*> call_list) {
    for (auto call : call_list) {
        if (call->get_floor() > current_floor) {
            return true;
            std::cout << "Request above" << std::endl;
        }
    }
    return false;
}

bool requests_below(int current_floor, std::vector<Call*> call_list) {
    for (auto call : call_list) {
        if (call->get_floor() < current_floor) {
            return true;
        }
    }
    return false;
}

bool requests_same_floor(int current_floor, std::vector<Call*> call_list) {
    for (auto call : call_list) {
        if (call->get_floor() == current_floor) {
            return true;
        }
    }
    return false;
}

bool should_stop(int current_floor, state_enum current_state, std::vector<Call*> call_list) {
    switch(current_state) {
        case state_enum::MOVING_UP:
            for (auto call : call_list) {
                if (
                    (call->get_call_type() == button_type::UP_HALL && call->get_floor() == current_floor) ||
                    (call->get_call_type() == button_type::CAB && call->get_floor() == current_floor) ||
                    !requests_above(current_floor, call_list)
                ) {
                    return true;
                }
            }
            return false;

        case state_enum::MOVING_DOWN:
            for (auto call : call_list) {
                if (
                    (call->get_call_type() == button_type::DOWN_HALL && call->get_floor() == current_floor) ||
                    (call->get_call_type() == button_type::CAB && call->get_floor() == current_floor) ||
                    !requests_below(current_floor, call_list)
                ) {
                    return true;
                }
            }
            return false;
            
        default:
            return false;
    }
}

int choose_direction(int current_floor, state_enum current_state, std::vector<Call*> call_list) {
    switch (current_state) {
        case state_enum::MOVING_UP:
            if (requests_above(current_floor, call_list)) {
                return 1;
            } else if (requests_below(current_floor, call_list)) {
                return -1;
            } else {
                return 0;
            }
        case state_enum::MOVING_DOWN:
            if (requests_below(current_floor, call_list)) {
                return -1;
            } else if (requests_above(current_floor, call_list)) {
                return 1;
            } else {
                return 0;
            }
        case state_enum::IDLE:
            if (requests_above(current_floor, call_list)) {
                return 1;
            } else if (requests_below(current_floor, call_list)) {
                return -1;
            } else {
                return 0;
            }
    }
    return 0;
}

/*
#pragma once
#include <vector>
#include <string>
#include <mutex>
#include <shared_mutex>
#include <map>

//enum for direction of elevator
enum class button_type {
    UP_HALL = 0,
    DOWN_HALL = 1,
    CAB = 2
};

struct Elevator_id {
    std::string id;
};

class Call_id {
    private:
        Elevator_id elevator_id;
        int call_number;

    public:

        Call_id(Elevator_id elevator_id, int call_number)
            : elevator_id(elevator_id), call_number(call_number) {};

        std::string get_call_id();
        Elevator_id get_call_elevator_id(){return elevator_id;}
        int get_call_number(){return call_number;};
};

// STATES
enum class state_enum {
    IDLE,
    MOVING_UP,
    MOVING_DOWN,
    DOOR_OPEN
};


class Call {
    private:

        static int floor;
        static button_type call_type;
        static Call_id call_id;
        static Elevator_id assigned_elevator; //maybe change to a bool called something like "assigned to local node"?
        
        std::vector<bool> serviced_ack_list;
        std::vector<Elevator_id> elevator_ack_list;


    public:
        Call(int floor, button_type call_type, Call_id call_id);


        std::vector<Elevator_id> get_elevator_ack_list();
        button_type get_call_type();
        Call_id get_call_id();
        int get_floor();


        void service_call(Elevator_id elevator_id);
};




















/*
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢨⣾⣿⣿⣷⣤⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣷⣪⣽⣿⣿⣦⣄⣀⢀⣀⣀⣀⢀⢀⣀⣠⣴⣶⣿⣿⣿⣿⠏
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣯⣿⣿⣿⣿⣿⣿⣿⡿⣿⣿⣿⡿⠃⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣤⣤⣤⣄⢠⢿⣽⣿⣿⣯⣿⣾⣶⣷⣿⣿⣿⣿⣿⣿⣿⣏⣻⣿⣿⣿⡟⠁⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣷⠀⠀⠀⠀⠈⣿⣿⣿⣿⣮⣿⣻⡯⣽⣿⣿⣾⣿⣿⣻⡿⢿⣫⣿⣾⣾⡗⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢻⠀⠀⠀⠀⠀⣯⡿⣿⣿⣿⣷⣾⣴⣽⣏⡄⢘⣿⣿⣿⣿⣿⣿⣿⠿⠋⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⠀⣿⣿⣦⣍⠟⡛⠿⣿⣿⣿⣿⣿⣿⠿⣿⣻⡏⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣷⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣤⡀⠀⠀⠉⠉⠀⣀⣴⣿⣿⣿⠅⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⡠⠤⠦⠤⠤⡇⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠿⣿⣻⡏⠀⠀⠀⠀⠀⠀⠀
⠀⠀⢀⣀⣀⣀⡅⠀⠀⠀⠀⠈⠀⠀⠀⠀⠀⡿⠿⠟⠿⠿⣿⣿⣿⠀⢀⣠⣿⣿⣿⣿⢿⡿⠂⠀⠀⠀⠀⠀⠀⠀
⠀⣠⠊⠀⠀⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠈⠉⠛⣿⣿⣿⣿⣿⠿⠃⠀⠀⠀⠀⠀⠀⠀⠀
⣰⠓⠀⠀⠀⣻⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣙⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⢹⡁⠀⠀⠀⢟⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⡾⠿⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⡜⢶⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣬⠟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠈⠹⢶⣆⡀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣰⡾⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠈⠛⠕⠲⠤⢤⢀⣀⣀⣀⣀⣀⣠⣤⡤⠼⠟⠛⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠁⠀⠀⠈⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
*/