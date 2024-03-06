#pragma once

#include "elevator_state_class.h"
#include "super_container.h"

class Elevator_state;
struct Elevator_id;

//elevator state functions
state_enum Elevator_state::get_current_state(){
    boost::unique_lock<boost::mutex> scoped_lock(mtx);
    return current_state;
}

int Elevator_state::get_current_floor(){
    boost::unique_lock<boost::mutex> scoped_lock(mtx);
    return current_floor;
}

bool Elevator_state::get_obstruction_status(){
    boost::unique_lock<boost::mutex> scoped_lock(mtx);
    return obstruction;
}

bool Elevator_state::get_alive_status(){
    boost::unique_lock<boost::mutex> scoped_lock(mtx);
    return alive;
}

Elevator_id Elevator_state::get_id(){
    return id;
}

elevator_status_network Elevator_state::get_status_network(){
    elevator_status_network status;
    boost::unique_lock<boost::mutex> scoped_lock(mtx);
    strncpy(status.id, id.id.c_str(), 8);
    status.state = static_cast<uint8_t>(current_state);
    status.floor = static_cast<int8_t>(current_floor);
    status.obstruction = obstruction;
    status.alive = alive;
    return status;
}


void Elevator_state::set_current_state(state_enum state){
    boost::unique_lock<boost::mutex> scoped_lock(mtx);
    current_state = state;
}

void Elevator_state::set_current_floor(int floor){
    boost::unique_lock<boost::mutex> scoped_lock(mtx);
    current_floor = floor;
}

void Elevator_state::set_obstruction(bool obstruction){
    boost::unique_lock<boost::mutex> scoped_lock(mtx);
    obstruction = obstruction;
}

void Elevator_state::set_alive(bool alive){
    boost::unique_lock<boost::mutex> scoped_lock(mtx);
    alive = alive;
}


std::string Elevator_state::get_behaviour() {
    std::string behaviour;
    switch (current_state) {
        case state_enum::IDLE:
            behaviour = "idle";
            break;
        case state_enum::MOVING_UP:
            behaviour = "moving";
            break;
        case state_enum::MOVING_DOWN:
            behaviour = "moving";
            break;
        case state_enum::DOOR_OPEN:
            behaviour = "doorOpen";
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

