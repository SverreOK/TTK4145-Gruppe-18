#include "call_class.h"

std::vector<Elevator_id> Call::get_elevator_ack_list() {
    return elevator_ack_list;
}

button_type Call::get_call_type() {
    return call_type;
}

Call_id Call::get_call_id() {
    return call_id;
}

int Call::get_floor() {
    return floor;
}
