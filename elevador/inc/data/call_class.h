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
         Elevator_id assigned_elevator; //maybe change to a bool called something like "assigned to local node"?
        
        std::vector<bool> serviced_ack_list;
        std::vector<Elevator_id> elevator_ack_list;


    public:
        Call(int floor, button_type call_type, Call_id* call_id);
        Call(int floor, button_type call_type, Call_id call_id);


        std::vector<Elevator_id> get_elevator_ack_list();
        button_type get_call_type();
        Call_id* get_call_id();
        int get_floor();
        bool is_serviced();

        void service_call(Elevator_id elevator_id);
        void acknowlegde_call(Elevator_id elevator_id);
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