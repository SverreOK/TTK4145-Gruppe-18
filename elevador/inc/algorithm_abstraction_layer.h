#include "elevator_fsm.h" // Elevator class should be defined in another file. Todo inheritance?

class Virtual_elevator {
    private:
        int elevator_ID;
        elevator_state elevator_state = {state_enum::IDLE, 0, false, std::vector<int8_t,int8_t>()}; // Todo beautify
        std::vector<Call> assigned_hall_calls;
        Call_database* call_database;
    public:
        Virtual_elevator(Elevator local_elevator);
        void set_current_state(state_enum state);
        void set_current_floor(int8_t floor);
        int get_elevator_ID();
        int get_current_floor();
        state_enum get_current_state();
        std::string get_current_behaviour();
        std::string get_current_direction();
        std::vector<bool> get_cab_call_floors();
};

std::vector<std::vector<bool>> call_list_to_floor_list(std::vector<Call> &calls);
Json::Value create_hall_request_json(std::vector<Virtual_elevator> &elevators, std::vector<Call> &calls) {
void reassign_calls(std::vector<Virtual_elevator> &elevators, std::vector<Call> &calls);
