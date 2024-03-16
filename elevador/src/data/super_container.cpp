#include "super_container.h"

#include "call_assigner.h"
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

std::vector<Call*> Super_container::update_locally_assigned_calls(){
    std::vector<Elevator_id> alive_elevators = get_alive_elevators();

    boost::unique_lock<boost::mutex> scoped_lock(mtx);

    std::vector<Call*> call_list_copy = call_list;
    std::vector<Call*> not_serviced_calls = std::vector<Call*>();

    //remove calls that are already serviced by checking if serviced vector length more than 0
    for (auto call : call_list_copy){

        std::vector<Elevator_id> call_ack_list = call->get_elevator_ack_list();

        if (call->get_serviced_ack_list().size() == 0 &&
            vector_elements_in_A_found_in_B(alive_elevators, call_ack_list)){
            not_serviced_calls.push_back(call);
        }
    }

    std::vector<Elevator_state*> elevators_copy = elevators;
    locally_assigned_calls = get_assigned_calls_for_elevator(not_serviced_calls, elevators, my_id);

    std::vector<Call*> copy =  locally_assigned_calls;
    return copy;
}

std::vector<Call*> Super_container::get_locally_assigned_calls(){
    boost::unique_lock<boost::mutex> scoped_lock(mtx);
    std::vector<Call*> copy = locally_assigned_calls;
    return copy;
}

Call* Super_container::add_call(Call* new_call){
    //TODO: should check is should merge? also merge in the if the call already exists
    boost::unique_lock<boost::mutex> scoped_lock(mtx);

    // std::vector<Call*> call_list_copy = call_list;
    //check if call with same ID exists
    Call* call_referenced = nullptr;

    bool already_exists = false;
    for (auto c : call_list){
        if (c->get_call_id()->call_number == new_call->get_call_id()->call_number &&
            c->get_call_id()->elevator_id.id == new_call->get_call_id()->elevator_id.id){



            for(auto elevator_id : new_call->get_elevator_ack_list()){
                c->acknowlegde_call(elevator_id);
            }

            for (auto elevator_id : new_call->get_serviced_ack_list()){
                c->service_call(elevator_id);
            }

            already_exists = true;
            call_referenced = c;
            break;

        }
    }

    if (!already_exists){
        call_list.push_back(new_call);
        call_referenced = new_call;
    }

    scoped_lock.unlock();

    update_locally_assigned_calls();
    push_new_call_event();

    return call_referenced;
}


void Super_container::add_new_call(int floor, button_type call_type, Call_id call_id){
    Call* new_call = new Call(floor, call_type, call_id);
    new_call->acknowlegde_call(call_id.elevator_id);
    add_call(new_call);
}

void Super_container::add_new_call_with_elevatorId(int floor, button_type call_type, Elevator_id elevator_id){
    if( !similar_call_exists(call_type, floor, elevator_id)){
        int call_num = 0;
        if (get_last_call_id_originating_from_elevator(elevator_id) != nullptr){
            call_num = get_last_call_id_originating_from_elevator(elevator_id)->call_number + 1;
        }

        Call_id new_call_id{elevator_id, call_num};
        add_new_call(floor, call_type, new_call_id);
    }
}

bool Super_container::similar_call_exists(button_type test_call_type, int test_floor, Elevator_id test_elevator_id){
    boost::unique_lock<boost::mutex> scoped_lock(mtx);
    //TODO CHECK ID IF CAB
    for (auto existing_call : call_list){ 
        button_type existing_call_type  = existing_call->get_call_type();
        bool is_local_cab_call = (existing_call_type == button_type::CAB) && (existing_call->get_call_id()->elevator_id.id == test_elevator_id.id);
        bool is_hall_call = !(existing_call->get_call_type() == button_type::CAB);

        if (is_local_cab_call || is_hall_call){
            if (existing_call->get_call_type() == test_call_type &&           //call has same type as provided button type
                test_floor == existing_call->get_floor() &&                   //call has same floor as provided nr
                existing_call->get_serviced_ack_list().size() == 0 ){    //call has empty serviced list

                return true;
            }
        }
    }
    return false;
}

std::vector<Call*> Super_container::get_calls_originating_from_elevator(Elevator_id elevator_id){
    std::vector<Call*> calls = std::vector<Call*>();

    boost::unique_lock<boost::mutex> scoped_lock(mtx);

    for (auto call : call_list){
        
        if (call->get_call_id()->elevator_id.id == elevator_id.id){
            calls.push_back(call);
        }
    }
    return calls;
}


Call_id* Super_container::get_last_call_id_originating_from_elevator(Elevator_id elevator_id){

    int last_call_id_number = 0;
    Call_id* last_call_id = nullptr;
    std::vector<Call*> calls = get_calls_originating_from_elevator(elevator_id);

    boost::unique_lock<boost::mutex> scoped_lock(mtx); 
    
    for (auto call : call_list){
        if (call->get_call_id()->call_number >= last_call_id_number){
            last_call_id = call->get_call_id();
            last_call_id_number = last_call_id->call_number;
        }
    }

    return last_call_id;
}


int Super_container::add_elevator(Elevator_state* elevator){
    boost::unique_lock<boost::mutex> scoped_lock(mtx);

    //check if the elevator is already in the list //TODO is this good?
    for (auto e : elevators){
        if (e->get_id().id == elevator->get_id().id){
            //update the elevator
            e->set_alive(true);
            e->set_current_state(elevator->get_current_state());
            e->set_current_floor(elevator->get_current_floor());
            e->set_obstruction(elevator->get_obstruction_status());
            e->set_last_seen();

            scoped_lock.unlock();
            update_locally_assigned_calls();

            return 1;
        }
    }

    elevators.push_back(elevator);

    scoped_lock.unlock();
    update_locally_assigned_calls();

    return 0;
}

void Super_container::remove_elevator(Elevator_id id){
    boost::unique_lock<boost::mutex> scoped_lock(mtx);

    for (auto elevator : elevators){
        if (elevator->get_id().id == id.id){
            elevators.erase(std::remove(elevators.begin(), elevators.end(), elevator), elevators.end());
        }
    }

    //loop through all calls, and remove the elevator from the serviced and ack list if it exists there
    for (auto call : call_list){
        call->remove_elevator_data(id);
    }

    scoped_lock.unlock();

    update_locally_assigned_calls();
    push_new_call_event();

}

void Super_container::set_my_id(Elevator_id id){
    boost::unique_lock<boost::mutex> scoped_lock(mtx);
    my_id = id;
}

std::vector<Elevator_id> Super_container::get_alive_elevators(){
    std::vector<Elevator_id> alive_elevators = std::vector<Elevator_id>();

    boost::unique_lock<boost::mutex> scoped_lock(mtx);
    
    for (auto elevator : elevators){
        if (elevator->get_alive_status()){
            alive_elevators.push_back(elevator->get_id());

        }
    }
    return alive_elevators;
}

Elevator_state* Super_container::get_elevator_by_id(Elevator_id id){
    boost::unique_lock<boost::mutex> scoped_lock(mtx);

    for (auto elevator : elevators){
        if (elevator->get_id().id == id.id){
            return elevator;
        }
    }

    return nullptr;
}

Elevator_id Super_container::get_my_id(){
    boost::unique_lock<boost::mutex> scoped_lock(mtx);
    return my_id;
}

void Super_container::push_new_call_event(){
    event_queue->push(elevator_event::ORDER_RECEIVED);
}

//TODO maybe update when fixing with networking
void Super_container::service_call(Call* call, Elevator_id elevator_id){

    call -> service_call(elevator_id);

    //check if the call serviced list has all elevators in elevators  vector in the serviced vector

    // std::vector<Elevator_id> serviced_list = call -> get_serviced_ack_list();
    //if all elevators are in the serviced list, remove the call from the call list
    // if (serviced_list.size() == elevators.size()){
    //     boost::unique_lock<boost::mutex> scoped_lock(mtx);
    //     call_list.erase(std::remove(call_list.begin(), call_list.end(), call), call_list.end()); //sjatt sjippidi idk if it works
    //     scoped_lock.unlock();
    // }

    update_locally_assigned_calls();

}

// std::vector<bool> Super_container::get_local_cab_requests_cheeky() {
//     std::vector<Call*> calls = get_calls_originating_from_elevator(my_id);
//     std::vector<bool> requests = {false, false, false, false};
//     for (auto call : calls) {
//         if (call->get_call_type() == button_type::CAB) {
//             requests[call->get_floor()] = true;
//         }
//     }
//     return requests;
// } // TODO



