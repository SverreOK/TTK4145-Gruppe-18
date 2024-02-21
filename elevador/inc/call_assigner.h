#pragma once

#include "elevator_state_class.h" 

std::vector<std::vector<bool>> call_list_to_floor_list(std::vector<Call*> calls);
std::string exec(const char* cmd);
std::string generate_hall_request_assigner_json(std::vector<std::vector<bool>> hall_call_floors, std::vector<Elevator_state*> elevators);
bool call_is_assigned(Call* call, std::vector<std::vector<bool>> assigned_floors);
std::vector<std::vector<bool>> get_assigned_floors_from_json(std::string json_string, std::string elevator_id);
std::vector<Call*> get_assigned_calls_from_json(std::string json_string, std::string elevator_id, std::vector<Call*> calls);
std::vector<Call*> get_assigned_calls_for_elevator(std::vector<Call*> calls, std::vector<Elevator_state*> elevators, Elevator_id local_id);