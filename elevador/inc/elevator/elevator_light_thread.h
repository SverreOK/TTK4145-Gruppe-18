#pragma once

#include <vector>

#include "super_container.h"

#include "elevator_driver.h"
#include <boost/thread.hpp>

class Light_controller {

    private:
        elevator_driver* driver;
        Super_container* data_container;
        Elevator_id elevator_id;
        bool running;
        int number_of_floors;
        int update_freq = 10; //hz

        //a 3 x number_of_floors matrix with bools to represent the current state of the lights
        std::vector<std::vector<bool>> light_matrix;
        std::vector<std::vector<bool>> temp_matrix;
    
    public:


    Light_controller(Super_container* data_container, elevator_driver* driver) 
                    : data_container(data_container), driver(driver) {}

    void Update_lights() {
        while (1)
        {

        temp_matrix = std::vector<std::vector<bool>>(3, std::vector<bool>(number_of_floors, false));
        //get the current call list and fill the temp matrix

        std::vector<Elevator_id> alive_elevators = data_container->get_alive_elevators();
        std::vector<Call*> call_list = data_container->get_call_list();
        for (auto call : call_list){
            for (auto elevator : alive_elevators){

                std::vector<Elevator_id> elev_ack_list = call->get_elevator_ack_list();

                for( auto ack : elev_ack_list){
                    if (ack.id == elevator.id){
                        temp_matrix[static_cast<int>( call->get_call_type() )]
                                   [call->get_floor()] = true;
                    }
                }
            }
        }

        //wait for x ms before updating the lights again
        //print in conside fuck my life :D
        std::cout << "Call list size: " << call_list.size() << std::endl;
        boost::this_thread::sleep_for(boost::chrono::milliseconds(1000));

        }
        
    }
};