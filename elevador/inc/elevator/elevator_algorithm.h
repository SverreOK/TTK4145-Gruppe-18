#include <stdbool.h>
#include <stdint.h>
#include <vector>
#include <iostream>
#include "elevator_fsm.h"
#include "elevator_driver.h"
#include "call_class.h"
#include "super_container.h"

bool requests_above(int current_floor, std::vector<Call*> call_list);

bool requests_below(int current_floor, std::vector<Call*> call_list);

bool requests_same_floor(int current_floor, std::vector<Call*> call_list);

int choose_direction(int current_floor, int current_direction, std::vector<Call*> call_list);

bool should_stop(int current_floor, int current_direction, std::vector<Call*> call_list);

int clear_at_current_floor(int current_floor, std::vector<Call*> call_list);

int clear_orders(int current_floor, std::vector<Call*> call_list);