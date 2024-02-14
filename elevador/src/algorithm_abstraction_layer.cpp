#include "algorithm_abstraction_layer.h"

Simulated_elevator::Simulated_elevator(ElevatorState state, int8_t floor) {
    currentState = state;
    currentFloor = floor;
    cab_call_floors = std::vector<std::vector<bool>>(N_FLOORS, std::vector<bool>(N_BUTTONS, false));
}

Simulated_elevator::Simulated_elevator(Elevator local_elevator) {
    // currentState = local_elevator.get_current_state();
    // currentFloor = local_elevator.get_current_floor();
    // cab_call_floors = local_elevator.get_cab_call_floors();
}

void Simulated_elevator::set_current_state(ElevatorState state) {
    currentState = state;
}

ElevatorState Simulated_elevator::get_current_state() {
    return currentState;
}

void Simulated_elevator::set_current_floor(int8_t floor) {
    currentFloor = floor;
}

int8_t Simulated_elevator::get_current_floor() {
    return currentFloor;
}

void Simulated_elevator::set_cab_call_floors(std::vector<bool> floors) {
    cab_call_floors = floors;
}

std::vector<bool> Simulated_elevator::get_cab_call_floors() {
    return cab_call_floors;
}

std::vector<std::vector<bool>> call_list_to_floor_list(std::vector<Call> &calls) {
    std::vector<std::vector<bool>> floors(N_FLOORS, std::vector<bool>(N_BUTTONS, false));
    for (auto call : calls) {
        switch (call.get_direction()) {
            case Direction::UP:
                floors[call.get_floor()][0] = true;
                break;
            case Direction::DOWN:
                floors[call.get_floor()][1] = true;
                break;
        }
    }
    return floors;
}

std::string create_hall_request_json(std::vector<Simulated_elevator> &elevators, std::vector<Call> &calls) {
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

void reassign_calls(std::vector<Simulated_elevator> &elevators, std::vector<Call> &calls) {
    std::string argument_string = create_hall_request_json(elevators, calls);
    std::string response = system("./hall_request_assigner --input '" + argument_string + "'");

    // Set call ownership based on returned json string
}