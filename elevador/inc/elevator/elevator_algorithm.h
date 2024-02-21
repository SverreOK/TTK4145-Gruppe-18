#include <stdbool.h>
#include <stdint.h>
#include "elevator_fsm.h"
#include "elevator_driver.h"
#include "call_class.h"

bool requests_above(Elevator *elevator, Elevator_state state);

bool requests_below(Elevator *elevator, Elevator_state state);

bool requests_same_floor(Elevator *elevator, Elevator_state state);

int choose_direction(Elevator *elevator, Elevator_state state);

bool should_stop(Elevator *elevator, Elevator_state state);

int clear_at_current_floor(Elevator *elevator, Elevator_state state);

int clear_orders(Elevator *elevator, Elevator_state state);
