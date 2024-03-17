#pragma once

#include <vector>
#include <boost/thread.hpp>

#include "super_container.h"
#include "elevator_driver.h"

class Light_controller {

    private:
        elevator_driver* driver;
        Super_container* data_container;
        Elevator_id elevator_id;
        bool running;
        int number_of_floors = NUM_FLOORSS;
        int update_freq = 420; //hz

        //a 3 x number_of_floors matrix with bools to represent the current state of the lights
        std::vector<std::vector<bool>> light_matrix = std::vector<std::vector<bool>>(3, std::vector<bool>(number_of_floors, false));
        
    public:
        Light_controller(Super_container* data_container, elevator_driver* driver);

        void Update_lights();
};