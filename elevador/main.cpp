#include "inc/elevator_driver.h"
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <vector>
#include <iostream>


int main(int argc, char *argv[]) {
    ElevatorDriver elevator;

    elevator.connect();

    int pollingrate_ms = 25;
    int floors = 4;
    int button_per_floor = 3;

    int light_status = 0;
    //loop through all floors and buttons and turn them on
    for (int floor = 0; floor < floors; ++floor) {
        for (int button = 0; button < button_per_floor; ++button) {
            elevator.set_button_lamp(button, floor, light_status);
        }
    }


    // Create a matrix to store the previous state of each button
    std::vector<std::vector<int>> button_states(floors, std::vector<int>(button_per_floor, 0));

    // Create a deadline_timer
    boost::asio::io_service io_service;
    boost::asio::deadline_timer timer(io_service, boost::posix_time::milliseconds(pollingrate_ms));

    // Create a function to be called when the timer expires
    std::function<void(const boost::system::error_code&)> check_buttons;

//     int i = 0;
//     while(true){
//         int state = elevator.get_button_signal(1, 2);
//         std::cout << state << " bruh " << i << '\n';

//         timer.expires_at(boost::posix_time::microsec_clock::universal_time() + boost::posix_time::milliseconds(pollingrate_ms));
//         timer.wait();
//         i++;
//    }
    elevator.set_motor_direction(1);
    boost::this_thread::sleep(boost::posix_time::seconds(1));
    elevator.set_motor_direction(0);


    while(true){
        for (int floor = 0; floor < floors; ++floor) {
            for (int button = 0; button < button_per_floor; ++button) {
                int state = elevator.get_button_signal(button, floor);
                if (state != button_states[floor][button]) {
                    std::cout << "Button " << button << " on floor " << floor << " changed state to " << state << std::endl;
                    button_states[floor][button] = state;
                    elevator.set_button_lamp(button, floor, state);
                }
            }
        }

        // Reset the timer //adasdasdasdasda
        timer.expires_at(boost::posix_time::microsec_clock::universal_time() + boost::posix_time::milliseconds(pollingrate_ms));
        timer.wait();
    }

    return 0;
}