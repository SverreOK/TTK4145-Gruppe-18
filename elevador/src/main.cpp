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

int main() {

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

    auto a = data_container->get_alive_elevators();
    auto b = data_container->get_call_list();


    //create a driver, poller and light controller
    elevator_driver* driver = new elevator_driver();

    driver->connect();

    // create fsm
    Elevator* elevator_fsm = new Elevator(driver, elevator_id, data_container, event_queue);
    elevator_fsm->start();

    Light_controller* light_controller = new Light_controller(data_container, driver);
    boost::thread light_thread(&Light_controller::Update_lights, light_controller);

    
    Elevator_driver_poller* poller = new Elevator_driver_poller(driver, elevator_id, data_container, 4, event_queue);
    boost::thread poller_thread(&Elevator_driver_poller::poll_all, poller);

    //create a peer
    Peer* peer = new Peer(data_container);
    boost::thread peer_thread(&Peer::infinite_status_broadcast, peer);
    boost::thread peer_thread2(&Peer::infinite_status_recieve, peer);
    boost::thread peer_thread3(&Peer::dead_connection_remover, peer);
    boost::thread peer_thread4(&Peer::infinite_call_recieve, peer);
    boost::thread peer_thread5(&Peer::infinite_call_transmit, peer);

    while(1){
        // std::cout << "Elevator is running" << std::endl;
        boost::this_thread::sleep_for(boost::chrono::milliseconds(1000));
    }

    return 0;
}
