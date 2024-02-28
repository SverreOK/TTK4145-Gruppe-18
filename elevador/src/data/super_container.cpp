#include "super_container.h"

class Call;

std::vector<Call*> Super_container::get_call_list(){
    //boost::unique_lock<boost::mutex> scoped_lock(mtx);
    std::vector<Call*> copy = call_list;
    return copy;
}

std::vector<Call*> Super_container::get_locally_assigned_calls(){
    //boost::unique_lock<boost::mutex> scoped_lock(mtx);
    std::vector<Call*> copy =  locally_assigned_calls;
    return copy;
}

Super_container::Super_container(){
    elevators = std::vector<Elevator_state*>();
    call_list = std::vector<Call*>();
    locally_assigned_calls = std::vector<Call*>();
}


void Super_container::add_call(int floor, button_type call_type, Call_id call_id){

    Call* new_call = new Call(floor, call_type, call_id);


    //boost::unique_lock<boost::mutex> scoped_lock(mtx);
    call_list.push_back(new_call);
}

void Super_container::add_call_with_elevatorId(int floor, button_type call_type, Elevator_id elevator_id){
    
    int call_num; //= get_last_call_id_originating_from_elevator(elevator_id)->call_number + 1;
    
    //Call_id* new_call_id = new Call_id(elevator_id, call_num);
    Call_id new_call_id{elevator_id, call_num};
    
    add_call(floor, call_type, new_call_id);

}

std::vector<Call*> Super_container::get_calls_originating_from_elevator(Elevator_id elevator_id){
    std::vector<Call*> calls;
    
    //boost::unique_lock<boost::mutex> scoped_lock(mtx);

    for (auto call : call_list){
        
        if (call->get_call_id()->elevator_id.id == elevator_id.id){
            calls.push_back(call);
        }
    }
    return calls;
}


// Function is commented for now because it throws an error. &call->get_call_id(); is a temporary value //fixed?
Call_id* Super_container::get_last_call_id_originating_from_elevator(Elevator_id elevator_id){
    //should make sure the returned call_id has the largest call_id.get_call_number()

    int last_call_id_number = 0;
    Call_id* last_call_id = nullptr;
    std::vector<Call*> calls = get_calls_originating_from_elevator(elevator_id);
    
    for (auto call : call_list){
        if (call->get_call_id()->call_number > last_call_id_number){
            last_call_id = call->get_call_id();
            last_call_id_number = last_call_id->call_number;
        }
    }

    return last_call_id;
}


void Super_container::add_elevator(Elevator_state* elevator){
    //boost::unique_lock<boost::mutex> scoped_lock(mtx);
    elevators.push_back(elevator);
}

std::vector<Elevator_id> Super_container::get_alive_elevators(){
    std::vector<Elevator_id> alive_elevators;

    //boost::unique_lock<boost::mutex> scoped_lock(mtx);
    for (auto elevator : elevators){
        if (elevator->get_alive_status()){
            alive_elevators.push_back(elevator->get_id());
        }
    }
    return alive_elevators;
}

Elevator_state* Super_container::get_elevator_by_id(Elevator_id id){
    //boost::unique_lock<boost::mutex> scoped_lock(mtx);
    for (auto elevator : elevators){
        if (elevator->get_id().id == id.id){
            return elevator;
        }
    }
    return nullptr;
}