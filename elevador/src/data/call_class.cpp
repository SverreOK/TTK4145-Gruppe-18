#include "call_class.h"
#include <cstring>

// class Call;
// class Call_id;
//TODO MISSING MUTEXES?
Call::Call(int floor, button_type call_type, Call_id* call_id) 
            : floor(floor), call_type(call_type), call_id(call_id){
}

Call::Call(int floor, button_type call_type, Call_id call_id) 
            : floor(floor), call_type(call_type){
    this->call_id = new Call_id(call_id);
}

Call::Call(call_message call_msg) 
            : floor(call_msg.floor), call_type(static_cast<button_type>(call_msg.call_type)){
    
    Elevator_id new_elevator_id{ std::string(call_msg.elevator_id) };
    int new_call_id_num = int{call_msg.call_id};
    this->call_id = new Call_id{new_elevator_id, new_call_id_num};

    std::vector<Elevator_id> elevator_ack_list;

    for (auto ack_elevator_id : call_msg.ack_list){
        std::string id_str(ack_elevator_id); //TODO: Double check that this is cast correctly in runtime
        //check that the id_str is not an empty string
        if (id_str != ""){
            elevator_ack_list.push_back(Elevator_id{id_str});
        }
    }
    
    this->elevator_ack_list = elevator_ack_list;
    std::vector<Elevator_id> serviced_ack_list;

    for (auto serviced_elevator_id : call_msg.serviced){
        //convert 8 char array to string
        std::string id_str(serviced_elevator_id); //TODO: Double check that this is cast correctly in runtime

        //check that the id_str is not an empty string
        if (id_str != ""){
            serviced_ack_list.push_back(Elevator_id{id_str});
        }
    }
    this->serviced_ack_list = serviced_ack_list;    
}

std::vector<Elevator_id> Call::get_elevator_ack_list() {
    return elevator_ack_list;
}

std::vector<Elevator_id> Call::get_serviced_ack_list() {
    return serviced_ack_list;
}

button_type Call::get_call_type() {
    return call_type;
}

Call_id* Call::get_call_id() {
    return call_id;
}

int Call::get_floor() {
    return floor;
}

void Call::acknowlegde_call(Elevator_id elevator_id) {

    //check if elevator_id is already in the list
    for (auto id : elevator_ack_list){
        if (id.id == elevator_id.id){
            return;
        }
    }
    
    elevator_ack_list.push_back(elevator_id);// elevator id is added to the list since it is not already there
}

void Call::service_call(Elevator_id elevator_id) {

    //check if elevator_id is already in the list
    for (auto id : serviced_ack_list){
        if (id.id == elevator_id.id){
            return;
        }
    }
    serviced_ack_list.push_back(elevator_id);
}

bool Call::is_serviced() {
    return elevator_ack_list.size() > 0;
}

call_message Call::get_call_message() {
    call_message call_msg;
    call_msg.floor = floor;
    call_msg.call_type = static_cast<uint8_t>(call_type);
    call_msg.call_id = call_id->call_number;
    strncpy(call_msg.elevator_id, call_id->elevator_id.id.c_str(), 8);
    

    for (size_t i = 0; i < elevator_ack_list.size(); i++) {
        strncpy(call_msg.ack_list[i], elevator_ack_list[i].id.c_str(), 8);
    }
    for (size_t i = elevator_ack_list.size(); i < NUM_ELEVATORS; i++) {
        strncpy(call_msg.ack_list[i], "", 8);
    }

    for (size_t i = 0; i < serviced_ack_list.size(); i++) {
        strncpy(call_msg.serviced[i], serviced_ack_list[i].id.c_str(), 8);
    }
    for (size_t i = serviced_ack_list.size(); i < NUM_ELEVATORS; i++) {
        strncpy(call_msg.serviced[i], "", 8);
    }

    return call_msg;

}