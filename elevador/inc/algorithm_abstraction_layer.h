#include "elevator_fsm.h" // Elevator class should be defined in another file. Todo inheritance?

class Simulated_elevator {
    private:
        elevator_state currentState;
        int currentFloor;
        std::vector<bool> cab_call_floors;
        int elevator_ID;
    public:
        Simulated_elevator(elevator_state state, int8_t floor);
        Simulated_elevator(Elevator local_elevator);
        void set_current_state(elevator_state state);
        void set_current_floor(int8_t floor);
        void set_cab_call_floors(std::vector<bool> floors);
        int get_elevator_ID();
        int get_current_floor();
        elevator_state get_current_state();
        std::vector<bool> get_cab_call_floors();
        std::string get_current_behaviour();
        std::string get_current_direction();
};

void reassign_calls(std::vector<Simulated_elevator> &elevators, std::vector<Call> &calls);