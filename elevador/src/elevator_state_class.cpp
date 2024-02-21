#include "elevator_state_class.h"

//elevator state functions
state_enum Elevator_state::get_current_state(){
    std::shared_lock<std::shared_mutex> lock(mutex);
    return current_state;
}

int Elevator_state::get_current_floor(){
    std::shared_lock<std::shared_mutex> lock(mutex);
    return current_floor;
}

bool Elevator_state::get_obstruction_status(){
    std::shared_lock<std::shared_mutex> lock(mutex);
    return obstruction;
}

bool Elevator_state::get_alive_status(){
    std::shared_lock<std::shared_mutex> lock(mutex);
    return alive;
}

Elevator_id Elevator_state::get_id(){
    return id;
}


void Elevator_state::set_current_state(state_enum state){
    std::unique_lock<std::shared_mutex> lock(mutex);
    current_state = state;
}

void Elevator_state::set_current_floor(int floor){
    std::unique_lock<std::shared_mutex> lock(mutex);
    current_floor = floor;
}

void Elevator_state::set_obstruction(bool obstruction){
    std::unique_lock<std::shared_mutex> lock(mutex);
    obstruction = obstruction;
}

void Elevator_state::set_alive(bool alive){
    std::unique_lock<std::shared_mutex> lock(mutex);
    alive = alive;
}


std::string Elevator_state::get_behaviour() {
    std::string behaviour;
    switch (current_state) {
        case state_enum::IDLE:
            behaviour = "idle";
            break;
        case state_enum::MOVING_UP:
        case state_enum::MOVING_DOWN:
            behaviour = "moving";
            break;
        case state_enum::DOOR_OPEN:
            behaviour = "door_open";
            break;
    }
    return behaviour;
}

std::string Elevator_state::get_direction() {
    std::string direction;
    switch (current_state) {
        case state_enum::IDLE:
            direction = "stop";
            break;
        case state_enum::MOVING_UP:
            direction = "up";
            break;
        case state_enum::MOVING_DOWN:
            direction = "down";
            break;
        case state_enum::DOOR_OPEN:
            direction = "stop";
            break;
    }
    return direction;
}



std::vector<bool> Elevator_state::get_cab_requests() {
    std::vector<bool> requests = {false, false, false, false};
    return requests;
} // TODO