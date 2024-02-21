#include "inc/elevator_algorithm.h"

// Check if there are requests above
static int requests_above(Elevator e) {
    for (int i = e.get_floor(); i < N_FLOORS; i++) {
        // if (e.get_requests[i][btn]) {
            //     return 1;
            // }
            // Need to have request list somewhere!!!
}

// Check if there are requests below
static int requests_below(Elevator e) {
    for (int i = 0; i < e.get_floor(); i++) {
        for (int btn = 0; btn < N_BUTTONS; btn++) {
            // if (e.get_requests[i][btn]) {
            //     return 1;
            // }
            // Need to have request list somewhere!!!
        }
    }
}
// Check if there are requests at current floor
static int requests_here(Elevator e) {
    for (int btn = 0; btn < N_BUTTONS; btn++) {
        // if (e.requests[e.get_floor()][btn]) {
        //     return 1;
        // }
    }
    return 0;
}

// Based on current requests, choose state moving_up, moving_down or idle
int choose_direction();

// During each floor and moving up or down see if you need to stop
int should_stop(Elevator e) {
    switch(e.get_state()) {
    case elevator_state::MOVING_UP:
        return 1;
            // e.requests[e.floor][B_HallUp]   ||
            // e.requests[e.floor][B_Cab]      ||
            // !requests_above(e)

    case elevator_state::MOVING_DOWN:
        return 1;
            // e.requests[e.floor][B_HallDown] ||
            // e.requests[e.floor][B_Cab]      ||
            // !requests_below(e);

    case elevator_state::STOP:
    
    default:
        return 0;
    }
}
// Should requests be cleared?
int should_clear_immediately();

// Clears request from the cab at the current floor
int clear_at_current_floor();

// Clear call strat
/*
When at floor you can assume that all calls that 
*/