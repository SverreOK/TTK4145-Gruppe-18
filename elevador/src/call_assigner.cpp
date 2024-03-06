#pragma once

#include "call_assigner.h"
#include <jsoncpp/json/json.h>
#include <iostream>

std::vector<std::vector<bool>> call_list_to_floor_list(std::vector<Call*> calls, int N_FLOORS) { // Output format: [[up_button, down_button], [up_button, down_button], ...]
    std::vector<std::vector<bool>> floors(N_FLOORS, std::vector<bool>(2, false));
    for (auto call : calls) {
        switch (call -> get_call_type()) {
            case button_type::UP_HALL:
                floors.at(call -> get_floor() )[0] = true; // -1 because the floor number is 1-indexed
                break;
            case button_type::DOWN_HALL:
                floors.at(call -> get_floor() )[1] = true;
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



std::string generate_hall_request_assigner_json(std::vector<std::vector<bool>> hall_call_floors, std::vector<Elevator_state*> elevators, std::vector<Call*> calls) { 
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

        
        std::vector<bool> temp_cab_requests = {false, false, false, false}; 
        for (auto call : calls) {
            if (call -> get_call_type() == button_type::CAB 
                && call -> get_call_id() -> elevator_id.id == elevator->get_id().id) {
                temp_cab_requests.at(call -> get_floor()) = true;
                }
        }

        Json::Value cabRequests(Json::arrayValue);
        for (bool request : temp_cab_requests) {
            cabRequests.append(request);
        }
        elevator_state["cabRequests"] = cabRequests;
        std::string elevator_id = elevator -> get_id().id; 
        states[elevator -> get_id().id] = elevator_state;
    }
    root["states"] = states;

    //std::string root_str = root.toStyledString(); // Styled string for easier debug (indentation and newlines)
    Json::FastWriter fastWriter;
    std::string root_str = fastWriter.write(root);
    root_str = root_str.substr(0, root_str.length() - 1);  // remove trailing newline
    Json::StyledWriter styledWriter;
    std::string root_str_2 = styledWriter.write(root);
    std::cout << root_str_2 << std::endl;
    return root_str;
}

bool call_is_assigned(Call* call, std::vector<std::vector<bool>> assigned_floors) {
    bool matches = false;

    bool going_up = call -> get_call_type() == button_type::UP_HALL;
    bool going_down = call -> get_call_type() == button_type::DOWN_HALL;
    int call_floor = call -> get_floor();

    if (assigned_floors.at(call_floor).at(0) && going_up 
        || assigned_floors.at(call_floor).at(1) && going_down) {
        matches = true;
    }

    return matches;
}

std::vector<std::vector<bool>> get_assigned_floors_from_json(std::string json_string, std::string elevator_id) {
    Json::Value root;
    Json::Reader reader;
    std::vector<std::vector<bool>> assigned_floors;

    bool parsingSuccessful = reader.parse(json_string, root);
    if (parsingSuccessful) {
        for (auto floor : root[elevator_id]) {
            std::vector<bool> floor_button_states;
            for (auto button : floor) {
                floor_button_states.push_back(button.asBool());
            }
            assigned_floors.push_back(floor_button_states);
        }
    }
    return assigned_floors;
}

std::vector<Call*> get_assigned_calls_from_json(std::string json_string, std::string elevator_id, std::vector<Call*> calls) {
    std::vector<std::vector<bool>> assigned_floors = get_assigned_floors_from_json(json_string, elevator_id);
    std::vector<Call*> assigned_calls;

    //check if assigned_calls has anything inside/is empty

    if (assigned_floors.empty()) {
        return assigned_calls;
    }

    for (auto call : calls) {
        if (call_is_assigned(call, assigned_floors)) {
            assigned_calls.push_back(call);
        }
    }
    return assigned_calls;
}

std::vector<Call*> get_assigned_calls_for_elevator(std::vector<Call*> calls, std::vector<Elevator_state*> elevators, Elevator_id local_id) {

    std::vector<std::vector<bool>> hall_call_floors = call_list_to_floor_list(calls, 4); // Set to 4 for now, should be replaced with a constant
    std::string argument_string = generate_hall_request_assigner_json(hall_call_floors, elevators, calls);
    std::string execution_string = "./hall_request_assigner --input '" + argument_string + "'";
    std::string hall_request_assigner_output = exec(execution_string.c_str());
    std::vector<Call*> assigned_calls = get_assigned_calls_from_json(hall_request_assigner_output, local_id.id, calls);

    return assigned_calls;
}