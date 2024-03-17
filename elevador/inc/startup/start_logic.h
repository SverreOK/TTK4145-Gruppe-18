#pragma once

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <vector>
#include <iostream>
#include <string>

#include "elevator_driver.h" 
#include "elevator_fsm.h"
#include "elevator_button_poller.h"
#include "elevator_light_thread.h"
#include "super_container.h"
#include "peer.h"
#include "debug_prints.h"

void launch_primary();

void launch_backup();

void launch_with_no_backup();
