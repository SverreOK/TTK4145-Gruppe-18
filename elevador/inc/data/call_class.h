#pragma once
#include <vector>
#include <string>
#include <map>
#include "config.h"
#include <boost/thread/mutex.hpp>

struct call_message {
    char elevator_id[8];
    uint8_t floor;
    uint8_t call_type;
    uint8_t call_id;
    char ack_list[NUM_ELEVATORS][8];
    char serviced[NUM_ELEVATORS][8];
};

enum class button_type {
    UP_HALL = 0,
    DOWN_HALL = 1,
    CAB = 2
};

struct Elevator_id {
    std::string id;
};

struct Call_id {
    Elevator_id elevator_id;
    int call_number;
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

        int floor;
        button_type call_type;
        Call_id* call_id;
        
        std::vector<Elevator_id> serviced_ack_list;
        std::vector<Elevator_id> elevator_ack_list;

        boost::mutex mtx;

    public:

        Call(int floor, button_type call_type, Call_id* call_id);
        Call(int floor, button_type call_type, Call_id call_id);
        Call(call_message call_msg);

        call_message get_call_message();

        std::vector<Elevator_id> get_elevator_ack_list();
        std::vector<Elevator_id> get_serviced_ack_list();
        button_type get_call_type();
        Call_id* get_call_id();
        int get_floor();
        bool is_serviced();

        void service_call(Elevator_id elevator_id);
        bool acknowlegde_call(Elevator_id elevator_id);
        void remove_elevator_data(Elevator_id id);
};

bool vector_elements_in_A_found_in_B(std::vector<Elevator_id> list1, std::vector<Elevator_id> list2);
bool vectors_are_equal(std::vector<Elevator_id> list1, std::vector<Elevator_id> list2);

















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
*/ // <3