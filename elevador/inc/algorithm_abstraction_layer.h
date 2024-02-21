#include "elevator_fsm.h" // Elevator class should be defined in another file. Todo inheritance?

std::vector<std::vector<bool>> call_list_to_floor_list(std::vector<Call> &calls);
Json::Value create_hall_request_json(std::vector<Virtual_elevator> &elevators, std::vector<Call> &calls) {
void reassign_calls(std::vector<Virtual_elevator> &elevators, std::vector<Call> &calls);
