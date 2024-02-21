#include "algorithm_abstraction_layer.h"
#include <jsoncpp/json/json.h> 

// Virtual_elevator::Virtual_elevator(elevator_state state, int8_t floor) {
//     current_state = state;
//     currentFloor = floor;
// }

// Virtual_elevator::Virtual_elevator(Elevator local_elevator) {
//     // elevator_state.current_state = local_elevator.get_state();
//     // elevator_state.current_floor = local_elevator.get_floor();
// }

void Virtual_elevator::set_current_state(state_enum state) {
    elevator_state.current_state = state;
}

void Virtual_elevator::set_current_floor(int8_t floor) {
    elevator_state.current_floor = floor;
}

int Virtual_elevator::get_elevator_ID() {
    return elevator_ID;
}

int Virtual_elevator::get_current_floor() {
    return elevator_state.current_floor;
}

state_enum Virtual_elevator::get_current_state() {
    return elevator_state.current_state;
}

std::string Virtual_elevator::get_current_behaviour() {
    switch (elevator_state.current_state) {
        case state_enum::MOVING_UP:
            return "moving";
        case state_enum::MOVING_DOWN:
            return "moving";
        case state_enum::DOOR_OPEN:
            return "doorOpen";
        default:
            return "idle";
    }
}

std::string Virtual_elevator::get_current_direction() {
    switch (elevator_state.current_state) {
        case state_enum::MOVING_UP:
            return "up";
        case state_enum::MOVING_DOWN:
            return "down";
        default:
            return "stop";
    }
}

std::vector<bool> Virtual_elevator::get_cab_call_floors() {
    std::vector<bool> floors(N_FLOORS, false);
    for (auto call : call_database -> get_call_list()) {
        if (call -> get_call_type() == button_type::CAB) {
            floors.at(call -> get_floor()) = true;
        }
    }
    return floors;
}

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

Json::Value create_hall_request_json(std::vector<Virtual_elevator> &elevators, std::vector<Call> &calls) {
    // Create the root of the JSON structure
    Json::Value root;

    // Populate the hallRequests array of arrays
    Json::Value hallRequests(Json::arrayValue);

    std::vector<std::vector<bool>> floors = call_list_to_floor_list(calls);
        for (auto floor : floors) {
            for (auto button : floor) {
                hallRequests.append(Json::Value(Json::arrayValue.append(button ? true : false)));
            }
        }
    //hallRequests.append(Json::Value(Json::arrayValue).append(false).append(true));
    root["hallRequests"] = hallRequests;

    // Create and populate the states object
    Json::Value states(Json::objectValue);

    for (auto elevator : elevators) {
        Json::Value state;
        state["behaviour"] = elevator.get_current_behaviour();
        state["floor"] = elevator.get_current_floor();
        state["direction"] = elevator.get_current_direction();

        Json::Value cabRequests(Json::arrayValue);
        for (auto button : elevator.get_cab_call_floors()) {
            cabRequests.append(button ? true : false);
        }
        state["cabRequests"] = cabRequests;

        // Use the elevator ID as the key
        states[std::to_string(elevator.get_elevator_ID())] = state;
    }

    root["states"] = states;

    // Output to demonstrate what we've built
    Json::StyledWriter writer;
    std::cout << writer.write(root) << std::endl;

    return root;
}

void reassign_calls(std::vector<Virtual_elevator> &elevators, std::vector<Call> &calls) {
    //std::string argument_string = create_hall_request_json(elevators, calls);
    Json::Value argument_json = create_hall_request_json(elevators, calls);
    
    std::string response = "";
    FILE* pipe = popen(("./hall_request_assigner --input '" + argument_json + "'").c_str(), "r"); // Run the hall_request_assigner program that was supplied 
    if (pipe) {
        char buffer[128];
        while (!feof(pipe)) {
            if (fgets(buffer, 128, pipe) != nullptr) {
                response += buffer;
            }
        }
        pclose(pipe);
    }

    Json::Value root;
    Json::Reader reader;
    bool parsingSuccessful = reader.parse(response, root);
    if (!parsingSuccessful) {
        std::cout << "Failed to parse the response" << std::endl;
        return;
    }

    std::map<Elevator_id, std::vector<bool>> elevator_responses;
    for (auto elevator : elevators) {
        elevator_responses[elevator.get_elevator_ID()] = obj[elevator.get_elevator_ID()];
    }
}

