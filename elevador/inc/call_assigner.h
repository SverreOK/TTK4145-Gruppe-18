#pragma once

#include "elevator_state_class.h" 

std::vector<Call*> get_assigned_calls_for_elevator(std::vector<Call*> calls, std::vector<Elevator_state*> elevators, Elevator_id local_id);