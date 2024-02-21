#include <stdbool.h>
#include <stdint.h>
#include "elevator_fsm.h"
#include "elevator_driver.h"
#include "call_class.h"

static bool requests_above(Elevator *elevator, elevator_state state);

static bool requests_below(Elevator *elevator, elevator_state state);

static bool requests_same_floor(Elevator *elevator, elevator_state state);

int choose_direction(Elevator *elevator, elevator_state state);

bool should_stop(Elevator *elevator, elevator_state state);

int clear_at_current_floor(Elevator *elevator, elevator_state state);

int clear_orders(Elevator *elevator, elevator_state state);
