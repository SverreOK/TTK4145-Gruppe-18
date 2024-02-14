#include "../inc/elevator_driver.h"

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <iostream>

elevator_driver::elevator_driver() : socket(io_service) {
    strncpy(ip, "localhost", sizeof(ip));
    strncpy(port, "15657", sizeof(port));
    elevator_driver::connected = false;
}

void elevator_driver::connect() {
    boost::asio::ip::tcp::resolver resolver(io_service);
    try {
        boost::asio::connect(socket, resolver.resolve({ip, port}));
    } catch (boost::system::system_error& e) {
        std::cout << "Failed to connect: " << e.what() << std::endl;
        return;
    }
    std::cout << "Connected successfully" << std::endl;
    
    elevator_driver::connected = true;
}

void elevator_driver::set_motor_direction(int dir) {
    std::lock_guard<std::mutex> lock(mtx);
    boost::asio::write(socket, boost::asio::buffer((char[4]) {1, dir}, 4));
}

void elevator_driver::set_button_lamp(int button, int floor, int value) {
    assert(floor >= 0);
    assert(floor < N_FLOORS);
    assert(button >= 0);
    assert(button < N_BUTTONS);

    std::lock_guard<std::mutex> lock(mtx);
    boost::asio::write(socket, boost::asio::buffer((char[4]) {2, button, floor, value}, 4));
}

void elevator_driver::set_floor_indicator(int floor) {
    assert(floor >= 0);
    assert(floor < N_FLOORS);

    std::lock_guard<std::mutex> lock(mtx);
    boost::asio::write(socket, boost::asio::buffer((char[4]) {3, floor}, 4));
}

void elevator_driver::set_door_open_lamp(int value) {
    std::lock_guard<std::mutex> lock(mtx);
    boost::asio::write(socket, boost::asio::buffer((char[4]) {4, value}, 4));
}

//todo add set_stop_light function!


int elevator_driver::get_button_signal(int button, int floor) {
    std::lock_guard<std::mutex> lock(mtx);
    boost::asio::write(socket, boost::asio::buffer((char[4]) {6, button, floor}, 4));
    char response[4];
    boost::asio::read(socket, boost::asio::buffer(response, 4));
    return response[1];
}

int elevator_driver::get_floor_sensor_signal() {
    std::lock_guard<std::mutex> lock(mtx);
    boost::asio::write(socket, boost::asio::buffer((char[4]) {7}, 4));
    char response[4];
    boost::asio::read(socket, boost::asio::buffer(response, 4));
    return response[1];
}

int elevator_driver::get_stop_signal() {
    std::lock_guard<std::mutex> lock(mtx);
    boost::asio::write(socket, boost::asio::buffer((char[4]) {8}, 4));
    char response[4];
    boost::asio::read(socket, boost::asio::buffer(response, 4));
    return response[1] ? response[2] : -1;
}

int elevator_driver::get_obstruction_signal() {
    std::lock_guard<std::mutex> lock(mtx);
    boost::asio::write(socket, boost::asio::buffer((char[4]) {9}, 4));
    char response[4];
    boost::asio::read(socket, boost::asio::buffer(response, 4));
    return response[1];
}

bool elevator_driver::get_connected() {
    return elevator_driver::connected;
}