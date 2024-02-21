#include "../inc/call_class.h"

std::vector<Call*> Call_database::get_call_list(){
    return call_list;
}

void Call_database::add_call(int floor, button_type call_type){
    Call* new_call = new Call(floor, call_type, call_list.size());
    call_list.push_back(new_call);
}