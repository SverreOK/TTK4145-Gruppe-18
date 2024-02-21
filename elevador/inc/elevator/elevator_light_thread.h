#pragma once

#include "../call_class.h"
#include "elevator_driver.h"
#include <boost/thread.hpp>



class light_controller {
private:
    elevator_driver* driver;
    Call_database* call_database;
    boost::thread poller_thread;
    Elevator_id elevator_id;
    bool running;
    int number_of_floors;
}