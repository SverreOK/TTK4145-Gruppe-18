#include "inc/data/super_container.h"


std::vector<Call*> Super_container::get_call_list(){
    return call_list;
}


void Super_container::add_call(int floor, button_type call_type, Call_id call_id){

    Call* new_call = new Call(floor, call_type, call_id);


    std::unique_lock<std::shared_mutex> lock(call_list_mutex);
    //call_list.push_back(new_call);
}

void Super_container::add_call_with_elevatorId(int floor, button_type call_type, Elevator_id elevator_id){
    
    int call_num = get_last_call_id_originating_from_elevator(elevator_id)->get_call_number() + 1;
    
    Call_id* new_call_id = new Call_id(elevator_id, call_num);
    add_call(floor, call_type, *new_call_id);

}

std::vector<Call*> Super_container::get_calls_originating_from_elevator(Elevator_id elevator_id){
    std::vector<Call*> calls;
    
    std::shared_lock<std::shared_mutex> lock(call_list_mutex);

    for (auto call : call_list){
        
        if (call->get_call_id().get_call_elevator_id().id == elevator_id.id){
            calls.push_back(call);
        }
    }
    return calls;
}

Call_id* Super_container::get_last_call_id_originating_from_elevator(Elevator_id elevator_id){
    //should make sure the returned call_id has the largest call_id.get_call_number()

    Call_id* last_call_id;
    std::vector<Call*> calls = get_calls_originating_from_elevator(elevator_id);
    
    for (auto call : call_list){
        if (call->get_call_id().get_call_number() > last_call_id->get_call_number()){
            last_call_id = &call->get_call_id();
        }
    }

    return last_call_id;
}
