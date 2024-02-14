#pragma once
#include "elevator_fsm.h"

static int requests_above();

static int requests_below();

static int requests_here();

int choose_direction();

int should_stop();

int should_clear_immediately();

int clear_at_current_floor();