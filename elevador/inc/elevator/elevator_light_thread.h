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
        int number_of_floors = 4; //TODO CONFIG
        int update_freq = 420; //hz

        //a 3 x number_of_floors matrix with bools to represent the current state of the lights
        std::vector<std::vector<bool>> light_matrix = std::vector<std::vector<bool>>(3, std::vector<bool>(number_of_floors, false));
        
    
    public:


    Light_controller(Super_container* data_container, elevator_driver* driver) 
                    : data_container(data_container), driver(driver) {}

    void Update_lights() {
        while (1)
        {

        std::vector<std::vector<bool>> temp_matrix = std::vector<std::vector<bool>>(3, std::vector<bool>(number_of_floors, false));
        //get the current call list and fill the temp matrix

        std::vector<Elevator_id> alive_elevators = data_container->get_alive_elevators();
        std::vector<Call*> call_list = data_container->get_call_list();
        for (auto call : call_list){
            int ack_count = 0;
            for (auto elevator : alive_elevators){

                std::vector<Elevator_id> elev_ack_list = call->get_elevator_ack_list();
                for( auto ack : elev_ack_list){
                    if (ack.id == elevator.id){
                        ack_count++;
                    }
                }
            }
            if (ack_count == alive_elevators.size() && 
                call->get_serviced_ack_list().size() == 0){
                    
                temp_matrix[static_cast<int>( call->get_call_type() )][call->get_floor()] = true;
            }
        }

        //compare the temp matrix with the light matrix and update the lights
        for (int floor = 0; floor < number_of_floors; ++floor) {
            for (int button = 0; button < 3; ++button) {
                if (temp_matrix[button][floor] != light_matrix[button][floor]){
                    driver->set_button_lamp(button, floor, temp_matrix[button][floor]);
                    light_matrix[button][floor] = temp_matrix[button][floor];
                }
            }
        }



        //wait for x ms before updating the lights again
        //print in conside fuck my life :D
        boost::this_thread::sleep_for(boost::chrono::milliseconds(1000));

        }
        
    }
};