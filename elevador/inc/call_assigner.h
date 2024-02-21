#pragma once

#include "inc/data/elevator_state_class.h" 

std::vector<std::vector<bool>> call_list_to_floor_list(std::vector<Call> &calls);
std::string generate_hall_request_assigner_json(std::vector<std::vector<bool>> hall_call_floors, std::vector<Elevator_state*> elevators);
void reassign_calls(std::vector<std::vector<bool>> hall_call_floors, std::vector<Elevator_state*> elevators);