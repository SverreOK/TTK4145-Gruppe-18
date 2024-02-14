#include "../inc/elevator_fsm.h" // Elevator class should be defined in another file. Todo inheritance?

class Virtual_elevator {
    private:
        int currentFloor;
        int elevator_ID;
        elevator_state currentState;
        std::vector<Call> assigned_hall_calls;
    public:
        Virtual_elevator(elevator_state state, int8_t floor);
        Virtual_elevator(Elevator local_elevator);
        void set_current_state(elevator_state state);
        void set_current_floor(int8_t floor);
        int get_elevator_ID();
        int get_current_floor();
        elevator_state get_current_state();
        std::string get_current_behaviour();
        std::string get_current_direction();
        std::vector<bool> get_cab_call_floors();
};

void reassign_calls(std::vector<Virtual_elevator> &elevators, std::vector<Call> &calls);