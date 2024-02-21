#include "inc/elevator/elevator_algorithm.h"

// N_FLOORS IS DEFINED IN DRIVERS
// button_type IS DEFINED IN CALL_CLASS

bool requests_above(Elevator *elevator, elevator_state *state) {
    for (int i = state->current_floor + 1; i < N_FLOORS; i++) {
        if (    state->orders[i][button_type::CAB] || 
                state->orders[i][button_type::DOWN_HALL] || 
                state->orders[i][button_type::UP_HALL]) {
            return true;
        }
    }
    return false;
}

bool requests_below(Elevator *elevator, elevator_state *state) {
    for (int i = state->current_floor - 1; i >= 0; i--) {
        if (    state->orders[i][button_type::CAB] || 
                state->orders[i][button_type::DOWN_HALL] || 
                state->orders[i][button_type::UP_HALL]) {
            return true;
        }
    }
    return false;
}

bool requests_same_floor(Elevator *elevator, elevator_state *state) {
    return  state->orders[state->current_floor][button_type::CAB] || 
            state->orders[state->current_floor][button_type::UP_HALL] || 
            state->orders[state->current_floor][button_type::DOWN_HALL];
}

bool should_stop(Elevator *elevator, elevator_state *state) {
    switch(state->current_state) {
        case state_enum::MOVING_UP:
            return
                state-> orders[state->current_floor][button_type::UP_HALL] ||
                state-> orders[state->current_floor][button_type::CAB] ||
                !requests_above(elevator, state);
        case state_enum::MOVING_DOWN:
            return
                state-> orders[state->current_floor][button_type::DOWN_HALL] ||
                state-> orders[state->current_floor][button_type::CAB] ||
                !requests_below(elevator, state);
        
        default:
            return false;
    }
}

int choose_direction(Elevator *elevator, elevator_state *state) {
    switch (state->current_state) {
        case state_enum::MOVING_UP:
            if (requests_above(elevator, state)) {
                return 1;
            } else if (requests_below(elevator, state)) {
                return -1;
            } else {
                return 0;
            }
        case state_enum::MOVING_DOWN:
            if (requests_below(elevator, state)) {
                return -1;
            } else if (requests_above(elevator, state)) {
                return 1;
            } else {
                return 0;
            }
        case state_enum::IDLE:
            if (requests_above(elevator, state)) {
                return 1;
            } else if (requests_below(elevator, state)) {
                return -1;
            } else {
                return 0;
            }
    }
    return 0;
}


void clear_at_current_floor(Elevator *elevator, elevator_state *state) {
    state->orders[state->current_floor][button_type::CAB] = 0;
    state->orders[state->current_floor][button_type::DOWN_HALL] = 0;
    state->orders[state->current_floor][button_type::UP_HALL] = 0;
}