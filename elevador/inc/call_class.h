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

class Call_database {
    private:

        std::map<Call_id, Call*> call_list;
        std::vector<Elevator_id> alive_elevators;

        std::shared_mutex call_list_mutex;

    public:

        std::map<Call_id, Call*>get_call_list();
        void add_call(int floor, button_type call_type, Call_id call_id);
        void add_call_with_elevatorId(int floor, button_type call_type, Elevator_id elevator_id);
        void change_call(Call* call, std::string elevator_id);

        std::map<Call_id, Call*> get_calls_originating_from_elevator(Elevator_id elevator_id);
        Call_id* get_last_call_id_originating_from_elevator(Elevator_id elevator_id);

        std::vector<Elevator_id> get_alive_elevators();
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