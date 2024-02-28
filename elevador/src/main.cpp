#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <vector>
#include <iostream>
#include <string>



#include "../inc/elevator/elevator_fsm.h"
#include "elevator_driver.h" 
#include "elevator_button_poller.h"
#include "elevator_light_thread.h"

#include "super_container.h"


int main() {

    //create an elevator_id
    Elevator_id elevator_id{"E1"};

    //create a super container
    Super_container* data_container = new Super_container();

    //add E1 to the super container
    Elevator_state* elevator = new Elevator_state(elevator_id);
    data_container->add_elevator(elevator);


    //create a driver, poller and light controller
    elevator_driver* driver = new elevator_driver();
    Elevator_driver_poller* poller = new Elevator_driver_poller(driver, elevator_id, data_container, 4);
    Light_controller* light_controller = new Light_controller(driver, elevator_id, data_container, 4);

    driver->connect();
    poller->start();
    light_controller->start();

    while(poller->get_running()){
        std::cout << "Elevator is running" << std::endl;
        boost::this_thread::sleep_for(boost::chrono::milliseconds(1000));
    }

    return 0;
}
