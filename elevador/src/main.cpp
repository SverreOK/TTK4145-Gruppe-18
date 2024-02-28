#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <vector>
#include <iostream>
#include <string>



#include "elevator_fsm.h"
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

    auto a = data_container->get_alive_elevators();
    auto b = data_container->get_call_list();



    //create a driver, poller and light controller
    elevator_driver* driver = new elevator_driver();

    driver->connect();

    Light_controller* light_controller = new Light_controller(data_container, driver);
    boost::thread light_thread(&Light_controller::Update_lights, light_controller);

    
    Elevator_driver_poller* poller = new Elevator_driver_poller(driver, elevator_id, data_container, 4);
    boost::thread poller_thread(&Elevator_driver_poller::poll_all, poller);


    
    while(1){
        std::cout << "Elevator is running" << std::endl;
        boost::this_thread::sleep_for(boost::chrono::milliseconds(1000));
    }

    return 0;
}
