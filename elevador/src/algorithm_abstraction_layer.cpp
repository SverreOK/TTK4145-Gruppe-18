#include "algorithm_abstraction_layer.h"
#include <jsoncpp/json/json.h> 

std::vector<std::vector<bool>> call_list_to_floor_list(std::vector<Call> &calls) {
    std::vector<std::vector<bool>> floors(N_FLOORS, std::vector<bool>(N_BUTTONS, false));
    for (auto call : calls) {
        switch (call.get_call_type()) {
            case button_type::UP_HALL:
                floors[call.get_floor()][0] = true;
                break;
            case button_type::DOWN_HALL:
                floors[call.get_floor()][1] = true;
                break;
            default:
                break;
        }
    }
    return floors;
}

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

std::string generate_hall_request_assigner_json(std::vector<std::vector<bool>> hall_call_floors, std::vector<Elevator_state*> elevators) { 
    Json::Value root;
    
    Json::Value hallRequests;
    for (auto floor : hall_call_floors) {
        Json::Value floor_buttons;
        for (bool button : floor) {
            floor_buttons.append(button);
        }
        hallRequests.append(floor_buttons);
    }
    root["hallRequests"] = hallRequests;

    Json::Value states;
    for (auto elevator : elevators) {
        Json::Value elevator_state;
        elevator_state["behaviour"] = elevator -> get_behaviour();
        elevator_state["floor"] = elevator -> get_current_floor();
        elevator_state["direction"] = elevator -> get_direction();
        Json::Value cabRequests;
        for (bool button : elevator -> get_cab_requests()) {
            cabRequests.append(button);
        }
        elevator_state["cabRequests"] = cabRequests;
        std::string elevator_id = elevator -> get_id().id; 
        states[elevator -> get_id().id] = elevator_state;
    }
    root["states"] = states;

    std::string root_str = root.toStyledString();

    return root_str;
}



void reassign_calls(std::vector<std::vector<bool>> hall_call_floors, std::vector<Elevator_state*> elevators) {

    std::string instruction_string = "./hall_request_assigner --input '" + generate_hall_request_assigner_json(hall_call_floors, elevators) + "'";
    // std::cout << instruction_string << std::endl;
    std::string output = exec(instruction_string.c_str());



    std::cout << output << std::endl;


}