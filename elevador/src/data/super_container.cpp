#include "super_container.h"

class Call;

Super_container::Super_container(thread_safe_queue* event_queue)
                                : event_queue(event_queue){
    elevators = std::vector<Elevator_state*>();
    call_list = std::vector<Call*>();
    locally_assigned_calls = std::vector<Call*>();
}

std::vector<Call*> Super_container::get_call_list(){
    boost::unique_lock<boost::mutex> scoped_lock(mtx);
    std::vector<Call*> copy = call_list;
    return copy;
}

std::vector<Call*> Super_container::get_locally_assigned_calls(){
    boost::unique_lock<boost::mutex> scoped_lock(mtx);
    std::vector<Call*> copy =  locally_assigned_calls;
    return copy;
}

void Super_container::add_call(Call* call){
    //TODO: should check is should merge? also merge in the if the call already exists
    boost::unique_lock<boost::mutex> scoped_lock(mtx);
    call_list.push_back(call);
    push_new_call_event();
}


void Super_container::add_new_call(int floor, button_type call_type, Call_id call_id){

    Call* new_call = new Call(floor, call_type, call_id);

    new_call->acknowlegde_call(call_id.elevator_id);

    printf("Adding call to call list\n");
    printf("Call id: %d\n", new_call->get_call_id()->call_number);

    add_call(new_call);
}

void Super_container::add_new_call_with_elevatorId(int floor, button_type call_type, Elevator_id elevator_id){

    if( !call_exists(call_type, floor)){
        int call_num = 0;
        if (get_last_call_id_originating_from_elevator(elevator_id) != nullptr){
            call_num = get_last_call_id_originating_from_elevator(elevator_id)->call_number + 1;
        }

        Call_id new_call_id{elevator_id, call_num};
        add_new_call(floor, call_type, new_call_id);
    }


}

bool Super_container::call_exists(button_type call_type, int floor){
    for (auto call : call_list){ //TODO ignore serviced calls!
        if (call->get_call_type() == call_type && floor == call->get_floor()){
            return true;
        }
    }
    return false;
}

std::vector<Call*> Super_container::get_calls_originating_from_elevator(Elevator_id elevator_id){
    std::vector<Call*> calls = std::vector<Call*>();
    

    for (auto call : call_list){
        
        if (call->get_call_id()->elevator_id.id == elevator_id.id){
            boost::unique_lock<boost::mutex> scoped_lock(mtx);
            calls.push_back(call);
            scoped_lock.unlock();   
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
    boost::unique_lock<boost::mutex> scoped_lock(mtx);
    elevators.push_back(elevator);
}

std::vector<Elevator_id> Super_container::get_alive_elevators(){
    std::vector<Elevator_id> alive_elevators = std::vector<Elevator_id>();

    
    for (auto elevator : elevators){
        if (elevator->get_alive_status()){
            boost::unique_lock<boost::mutex> scoped_lock(mtx);
            alive_elevators.push_back(elevator->get_id());
            scoped_lock.unlock();
        }
    }
    return alive_elevators;
}

Elevator_state* Super_container::get_elevator_by_id(Elevator_id id){
    
    for (auto elevator : elevators){
        if (elevator->get_id().id == id.id){
            boost::unique_lock<boost::mutex> scoped_lock(mtx);
            return elevator;
            scoped_lock.unlock();
        }
    }
    return nullptr;
}

Elevator_id Super_container::get_my_id(){
    return my_id;
}

void Super_container::push_new_call_event(){
    event_queue->push(elevator_event::ORDER_RECEIVED);
}

//TODO maybe update when fixing with networking
void Super_container::service_call(Call* call, Elevator_id elevator_id){
    //find the call in the call list and run the service call function
    for (auto c : call_list){
        if (c->get_call_id()->call_number == call->get_call_id()->call_number){
            c->service_call(elevator_id);
        }
    }

    //check if the call serviced list has all elevators in the elevators vector

    std::vector<Elevator_id> serviced_calls = call->get_elevator_ack_list();

    if (serviced_calls.size() == elevators.size()){
        //remove the call from the call list
        for (int i = 0; i < call_list.size(); i++){
            if (call_list[i]->get_call_id()->call_number == call->get_call_id()->call_number){
                boost::unique_lock<boost::mutex> scoped_lock(mtx);
                call_list.erase(call_list.begin() + i);
                scoped_lock.unlock();
            }
        }
    }

}