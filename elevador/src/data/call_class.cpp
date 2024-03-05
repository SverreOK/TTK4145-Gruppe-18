#include "call_class.h"

class Call;
class Call_id;

Call::Call(int floor, button_type call_type, Call_id* call_id) 
            : floor(floor), call_type(call_type), call_id(call_id){
}

Call::Call(int floor, button_type call_type, Call_id call_id) 
            : floor(floor), call_type(call_type){
    this->call_id = new Call_id(call_id);
}

Call::Call(call_message call_msg) 
            : floor(call_msg.floor), call_type(static_cast<button_type>(call_msg.call_type)){
    Call_id new_call_id{Elevator_id{call_msg.elevator_id}, call_msg.call_id};
    this->call_id = new Call_id(new_call_id);
    std::vector<Elevator_id> elevator_ack_list;
    for (auto elevator_id : call_msg.ack_list){
        //convert 8 char array to string
        std::string id_str(call_msg.elevator_id); //TODO: Double check that this is cast correctly in runtime
        elevator_ack_list.push_back(Elevator_id{id_str});
    }
    this->elevator_ack_list = elevator_ack_list;
    std::vector<Elevator_id> serviced_ack_list;
    for (auto elevator_id : call_msg.serviced){
        //convert 8 char array to string
        std::string id_str(call_msg.elevator_id); //TODO: Double check that this is cast correctly in runtime
        serviced_ack_list.push_back(Elevator_id{id_str});
    }
    this->serviced_ack_list = serviced_ack_list;    
}

std::vector<Elevator_id> Call::get_elevator_ack_list() {
    return elevator_ack_list;
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
    elevator_ack_list.push_back(elevator_id);
}

void Call::service_call(Elevator_id elevator_id) {
    serviced_ack_list.push_back(elevator_id);
}

bool Call::is_serviced() {
    return elevator_ack_list.size() > 0;
}