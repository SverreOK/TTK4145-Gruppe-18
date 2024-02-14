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

void Simulated_elevator::set_cab_call_floors(std::vector<std::vector<bool>> floors) {
    cab_call_floors = floors;
}

std::vector<std::vector<bool>> Simulated_elevator::get_cab_call_floors() {
    return cab_call_floors;
}

void reassign_calls(std::vector<Simulated_elevator> &elevators, std::vector<Call> &calls) {
    std::string argument_string = "";
    // Create argument string [input json] based on elevators and calls
    // Run d program with argument string
    // Set call ownership based on returned json string
}