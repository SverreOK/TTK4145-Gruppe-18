#pragma once

#include <vector>
#include "inc/call_class.h"
#include "inc/elevator/elevator_driver.h"
#include <boost/thread.hpp>



class light_controller {

    private:
        elevator_driver* driver;
        Call_database* call_database;
        boost::thread light_thread;
        Elevator_id elevator_id;
        bool running;
        int number_of_floors;

        //a 3 x number_of_floors matrix with bools to represent the current state of the lights
        std::vector<std::vector<bool>> light_matrix;
        std::vector<std::vector<bool>> temp_matrix;


        void update_lights() {
            while (running) {
                
                temp_matrix = std::vector<std::vector<bool>>(3, std::vector<bool>(number_of_floors, false));
                //get the current call list and fill the temp matrix
                std::vector<Call*> call_list = call_database->get_call_list();
                std::vector<Elevator_id> alive_elevators = call_database->get_alive_elevators();
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
            }
        }

    public:
        light_controller(elevator_driver* driver, Elevator_id elevator_id) 
            : driver(driver), running(false), elevator_id(elevator_id) {
                number_of_floors = 4;
                light_matrix = std::vector<std::vector<bool>>(3, std::vector<bool>(number_of_floors, false));
            }
        
        void start() {
            running = true;
            light_thread = boost::thread(&light_controller::update_lights, this);
        }

        void stop() {
            running = false;
            if (light_thread.joinable()) {
                light_thread.join();
            }
        }


};