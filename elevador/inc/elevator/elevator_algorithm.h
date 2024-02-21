#include <stdbool.h>
#include <stdint.h>
#include <vector>
#include "elevator_fsm.h"
#include "elevator_driver.h"
#include "inc/data/call_class.h"

#include "inc/data/super_container.h"

bool requests_above(int current_floor, std::vector<Call*> call_list);

bool requests_below(int current_floor, std::vector<Call*> call_list);

bool requests_same_floor(int current_floor, std::vector<Call*> call_list);

int choose_direction(int current_floor, state_enum current_state, std::vector<Call*> call_list);

bool should_stop(int current_floor, state_enum current_state, std::vector<Call*> call_list);

int clear_at_current_floor(int current_floor, std::vector<Call*> call_list);

int clear_orders(int current_floor, std::vector<Call*> call_list);