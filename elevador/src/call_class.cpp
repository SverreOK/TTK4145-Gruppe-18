#include "../inc/call_class.h"

std::map<Call_id, Call*> Call_database::get_call_list(){
    return call_list;
}


void Call_database::add_call(int floor, button_type call_type, Call_id call_id){

    Call* new_call = new Call(floor, call_type, call_id);


    std::unique_lock<std::shared_mutex> lock(call_list_mutex);
    //call_list.push_back(new_call);
}

void Call_database::add_call_with_elevatorId(int floor, button_type call_type, Elevator_id elevator_id){
    
    int call_num = get_last_call_id_originating_from_elevator(elevator_id)->get_call_number() + 1;
    
    Call_id* new_call_id = new Call_id(elevator_id, call_num);
    add_call(floor, call_type, *new_call_id);

}

std::map<Call_id, Call*>Call_database::get_calls_originating_from_elevator(Elevator_id elevator_id){
    std::map<Call_id, Call*> calls;
    
    std::shared_lock<std::shared_mutex> lock(call_list_mutex);

    for (auto& [call_id, call] : call_list){
        
        if (call->get_call_id().get_call_elevator_id().id == elevator_id.id){
            calls.insert(std::pair<Call_id, Call*>(call_id, call));
        }
    }
    return calls;
}

Call_id* Call_database::get_last_call_id_originating_from_elevator(Elevator_id elevator_id){
    //should make sure the returned call_id has the largest call_id.get_call_number()

    Call_id* last_call_id;
    std::map<Call_id, Call*> calls = get_calls_originating_from_elevator(elevator_id);
    
    for (auto& [call_id, call] : call_list){
        if (call->get_call_id().get_call_number() > last_call_id->get_call_number()){
            last_call_id = &call->get_call_id();
        }
    }

    return last_call_id;
}


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


