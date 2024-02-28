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
