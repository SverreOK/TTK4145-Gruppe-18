#include "elevator_fsm.h" // Elevator class should be defined in another file. Todo inheritance?

class Simulated_elevator {
    private:
        ElevatorState currentState;
        int8_t currentFloor;
        std::vector<std::vector<bool>> cab_call_floors;
    public:
        Simulated_elevator(ElevatorState state, int8_t floor);
        Simulated_elevator(Elevator local_elevator);
        void set_current_state(ElevatorState state);
        ElevatorState get_current_state();
        void set_current_floor(int8_t floor);
        int8_t get_current_floor();
        void set_cab_call_floors(std::vector<std::vector<bool>> floors);
        std::vector<std::vector<bool>> get_cab_call_floors();
};

void reassign_calls(std::vector<Simulated_elevator> &elevators, std::vector<Call> &calls);