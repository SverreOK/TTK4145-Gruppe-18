#pragma once

#define BOOST_THREAD_USE_LIB
#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <mutex>

// Number of floors. Hardware-dependent, do not modify.
#define N_FLOORS 4

// Number of buttons (and corresponding lamps) on a per-floor basis
#define N_BUTTONS 3




////////////////////////////////////////////////////////////////////////
///////////////////////// Elevator Driver  /////////////////////////////
////////////////////////////////////////////////////////////////////////

class ElevatorDriver {
private:
    boost::asio::io_service io_service;
    boost::asio::ip::tcp::socket socket;

    std::mutex mtx;

    char ip[16];
    char port[8];
    bool connected;
    

public:
    ElevatorDriver();
    void connect();

    void set_motor_direction(int dir);
    void set_button_lamp(int button, int floor, int value);
    void set_floor_indicator(int floor);
    void set_door_open_lamp(int value);
    void set_stop_lamp(int value);

    int get_button_signal(int button, int floor);
    int get_floor_sensor_signal();
    int get_stop_signal();
    int get_obstruction_signal();
    
    bool get_connected();

};





//Do we need these? / use them?

// typedef enum tag_elevator_hardware_motor_direction { 
//     DIRN_DOWN = -1,
//     DIRN_STOP = 0,
//     DIRN_UP = 1
// } elevator_hardware_motor_direction_t;

// typedef enum tag_elevator_hardware_lamp_type { 
//     BUTTON_CALL_UP = 0,
//     BUTTON_CALL_DOWN = 1,
//     BUTTON_COMMAND = 2
// } elevator_hardware_button_type_t;