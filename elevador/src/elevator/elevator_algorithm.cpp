#include "elevador/inc/elevator/elevator_algorithm.h"

int requests_above(Elevator *elevator, elevator_state *state) {
    for (int i = state->current_floor + 1; i < FLOORS; i++) {
        if (state->orders[i][0] || state->orders[i][1] || state->orders[i][2]) {
            return true;
        }
    }
    return false;
}

int requests_below(Elevator *elevator, elevator_state *state) {
    for (int i = 0; i < state->current_floor; i++) {
        if (state->orders[i][0] || state->orders[i][1] || state->orders[i][2]) {
            return true;
        }
    }
    return false;
}

int requests_same_floor(Elevator *elevator, elevator_state *state) {
    return state->orders[state->current_floor][0] || state->orders[state->current_floor][1] || state->orders[state->current_floor][2];
}

int should_stop(Elevator *elevator, elevator_state *state) {
    switch(state->current_state) {
        case MOVING_UP:
            return
                state-> orders[state->current_floor][B_HallUp] ||
                state-> orders[state->current_floor][B_Cab] ||
                !requests_above(elevator, state);
        case MOVING_DOWN:
            return
                state-> orders[state->current_floor][B_HallDown] ||
                state-> orders[state->current_floor][B_Cab] ||
                !requests_below(elevator, state);
        
        default:
            return 0;
    }
}

int choose_direction(Elevator *elevator, elevator_state *state) {
    
}


void clear_at_current_floor(Elevator *elevator, elevator_state *state) {
    state->orders[state->current_floor][1] = 0;
    state->orders[state->current_floor][2] = 0;
    state->orders[state->current_floor][3] = 0;
}