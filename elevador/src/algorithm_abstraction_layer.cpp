#include "algorithm_abstraction_layer.h"

Virtual_elevator::Virtual_elevator(elevator_state state, int8_t floor) {
    currentState = state;
    currentFloor = floor;
}

Virtual_elevator::Virtual_elevator(Elevator local_elevator) {
    currentState = local_elevator.get_state();
    currentFloor = local_elevator.get_floor();
}

void Virtual_elevator::set_current_state(elevator_state state) {
    currentState = state;
}

void Virtual_elevator::set_current_floor(int8_t floor) {
    currentFloor = floor;
}

int Virtual_elevator::get_elevator_ID() {
    return elevator_ID;
}

int Virtual_elevator::get_current_floor() {
    return currentFloor;
}

elevator_state Virtual_elevator::get_current_state() {
    return currentState;
}

std::string Virtual_elevator::get_current_behaviour() {
    switch (currentState) {
        case elevator_state::MOVING_UP:
            return "moving";
        case elevator_state::MOVING_DOWN:
            return "moving";
        case elevator_state::DOOR_OPEN:
            return "doorOpen";
        default:
            return "idle";
    }
}

std::string Virtual_elevator::get_current_direction() {
    switch (currentState) {
        case elevator_state::MOVING_UP:
            return "up";
        case elevator_state::MOVING_DOWN:
            return "down";
        default:
            return "stop";
    }
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

std::string create_hall_request_json(std::vector<Virtual_elevator> &elevators, std::vector<Call> &calls) {
    std::string argument_string = "{\n\t\"hallRequests\" : \n\t\t[";
        std::vector<std::vector<bool>> floors = call_list_to_floor_list(calls);
        for (auto floor : floors) {
            argument_string += "[";
            for (auto button : floor) {
                argument_string += button ? "true" : "false";
                argument_string += ",";
            }
            argument_string.pop_back(); // Remove trailing comma
            argument_string += "],";
        }
        argument_string += "],\n\t\"states\" : {";
        for (auto elevator : elevators) {
            argument_string += "\n\t\t\"" + std::to_string(elevator.get_elevator_ID()) + "\" : {\n";
                argument_string += "\t\t\t\"behaviour\":\"" + elevator.get_current_behaviour() + "\",\n";
                argument_string += "\t\t\t\"floor\":" + std::to_string(elevator.get_current_floor()) + ",\n";
                argument_string += "\t\t\t\"direction\":\"" + elevator.get_current_direction() + "\",\n";
                argument_string += "\t\t\t\"cabRequests\":[";
                for (auto button : elevator.get_cab_call_floors()) {
                    argument_string += button ? "true" : "false";
                    argument_string += ",";
                }
                argument_string.pop_back(); // Remove trailing comma
                argument_string += "]\n";
            argument_string += "\t\t},";
        }
        argument_string.pop_back(); // Remove trailing comma
        argument_string += "\n\t}\n";
    argument_string += "}";

    return argument_string;
}

void reassign_calls(std::vector<Virtual_elevator> &elevators, std::vector<Call> &calls) {
    std::string argument_string = create_hall_request_json(elevators, calls);
    std::string response = "";
    FILE* pipe = popen(("./hall_request_assigner --input '" + argument_string + "'").c_str(), "r");
    if (pipe) {
        char buffer[128];
        while (!feof(pipe)) {
            if (fgets(buffer, 128, pipe) != nullptr) {
                response += buffer;
            }
        }
        pclose(pipe);
    }

    // Set call ownership based on returned json string
}