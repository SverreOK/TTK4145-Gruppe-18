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

void launch_primary() {
    //create an elevator_id
    Elevator_id elevator_id{ELEVATOR_ID};

    //create event queue
    thread_safe_queue* event_queue = new thread_safe_queue();

    //create a super container
    Super_container* data_container = new Super_container(event_queue);

    //add E1 to the super container
    Elevator_state* elevator = new Elevator_state(elevator_id);
    elevator->set_alive(true);
    data_container->add_elevator(elevator);
    data_container->set_my_id(elevator_id);

    //create a driver, poller and light controller
    elevator_driver* driver = new elevator_driver();

    driver->connect();

    // create fsm
    Elevator* elevator_fsm = new Elevator(driver, elevator_id, data_container, event_queue);
    elevator_fsm->start();

    Light_controller* light_controller = new Light_controller(data_container, driver);
    boost::thread light_thread(&Light_controller::Update_lights, light_controller);

    
    Elevator_driver_poller* poller = new Elevator_driver_poller(driver, elevator_id, data_container, 4, event_queue, elevator_fsm);
    boost::thread poller_thread(&Elevator_driver_poller::poll_all, poller);

    //create a peer
    Peer* peer = new Peer(data_container);
    peer->run_peer();

    // debug print
    Debug_prints* debug_prints = new Debug_prints(data_container, elevator_id, event_queue);
    boost::thread debug_thread(&Debug_prints::debug_print_start, debug_prints, data_container, elevator_id, event_queue);
}

void launch_backup() {
    std::string command = "gnome-terminal -- bash -c './main backup; exec bash'";
    std::system(command.c_str());
}

void launch_with_no_backup() {
    //create an elevator_id
    Elevator_id elevator_id{ELEVATOR_ID};

    //create event queue
    thread_safe_queue* event_queue = new thread_safe_queue();

    //create a super container
    Super_container* data_container = new Super_container(event_queue);

    //add E1 to the super container
    Elevator_state* elevator = new Elevator_state(elevator_id);
    elevator->set_alive(true);
    data_container->add_elevator(elevator);
    data_container->set_my_id(elevator_id);

    //create a driver, poller and light controller
    elevator_driver* driver = new elevator_driver();

    driver->connect();

    // create fsm
    Elevator* elevator_fsm = new Elevator(driver, elevator_id, data_container, event_queue);
    elevator_fsm->start();

    Light_controller* light_controller = new Light_controller(data_container, driver);
    boost::thread light_thread(&Light_controller::Update_lights, light_controller);

    
    Elevator_driver_poller* poller = new Elevator_driver_poller(driver, elevator_id, data_container, 4, event_queue, elevator_fsm);
    boost::thread poller_thread(&Elevator_driver_poller::poll_all, poller);

    //create a peer
    Peer* peer = new Peer(data_container);
    peer->run_peer();

    // debug print
    Debug_prints* debug_prints = new Debug_prints(data_container, elevator_id, event_queue);
    boost::thread debug_thread(&Debug_prints::debug_print_start, debug_prints, data_container, elevator_id, event_queue);
    

    while(1){
        // std::cout << "Elevator is running" << std::endl;
        boost::this_thread::sleep_for(boost::chrono::milliseconds(1000));
    }
}
