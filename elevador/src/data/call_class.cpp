#include "call_class.h"
#include <cstring>

bool vector_elements_in_A_found_in_B(std::vector<Elevator_id> list1, std::vector<Elevator_id> list2) {

    if (list1.size() == 0) {
        return true;
    }

    for (auto item1 : list1) {
        bool found = false;
        for (auto item2 : list2) {
            if (item1.id == item2.id) {
                found = true;
                break;
            }
        }
        if (!found) {
            return false;
        }
    }
    return true;
}

bool vectors_are_equal(std::vector<Elevator_id> list1, std::vector<Elevator_id> list2) {
    if (list1.size() != list2.size()) {
        return false;
    }

    if (vector_elements_in_A_found_in_B(list1, list2) &&
        vector_elements_in_A_found_in_B(list2, list1)){
        return true;
    }
    
    return false;
}

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
    boost::unique_lock<boost::mutex> scoped_lock(mtx);
    return elevator_ack_list;
}

std::vector<Elevator_id> Call::get_serviced_ack_list() {
    boost::unique_lock<boost::mutex> scoped_lock(mtx);
    return serviced_ack_list;
}

button_type Call::get_call_type() {
    boost::unique_lock<boost::mutex> scoped_lock(mtx);
    return call_type;
}

Call_id* Call::get_call_id() {
    boost::unique_lock<boost::mutex> scoped_lock(mtx);
    return call_id;
}

int Call::get_floor() {
    boost::unique_lock<boost::mutex> scoped_lock(mtx);
    return floor;
}

bool Call::acknowlegde_call(Elevator_id elevator_id) { //TODO idk if this thing needs to return anything

    boost::unique_lock<boost::mutex> scoped_lock(mtx);

    //check if elevator_id is already in the list
    for (auto id : elevator_ack_list){
        if (id.id == elevator_id.id){
            return true;
        }
    }
    
    elevator_ack_list.push_back(elevator_id);
    return false;
}

void Call::service_call(Elevator_id elevator_id) {

    boost::unique_lock<boost::mutex> scoped_lock(mtx);

    //check if elevator_id is already in the list
    for (auto id : serviced_ack_list){
        if (id.id == elevator_id.id){
            return;
        }
    }
    serviced_ack_list.push_back(elevator_id);
}

bool Call::is_serviced() {
    boost::unique_lock<boost::mutex> scoped_lock(mtx);
    return elevator_ack_list.size() > 0;
}

call_message Call::get_call_message() {
    boost::unique_lock<boost::mutex> scoped_lock(mtx);
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

void Call::remove_elevator_data(Elevator_id id){
    boost::unique_lock<boost::mutex> scoped_lock(mtx);

    for (auto it = elevator_ack_list.begin(); it != elevator_ack_list.end(); it++){
        if (it->id == id.id){
            elevator_ack_list.erase(it);
            break;
        }
    }

    for (auto it = serviced_ack_list.begin(); it != serviced_ack_list.end(); it++){
        if (it->id == id.id){
            serviced_ack_list.erase(it);
            break;
        }
    }
}