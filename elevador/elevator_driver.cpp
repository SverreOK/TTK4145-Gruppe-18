#include "elevator_driver.h"

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <iostream>

ElevatorDriver::ElevatorDriver() : socket(io_service) {
    strncpy(ip, "localhost", sizeof(ip));
    strncpy(port, "15657", sizeof(port));
}

void ElevatorDriver::connect() {
    boost::asio::ip::tcp::resolver resolver(io_service);
    try {
        boost::asio::connect(socket, resolver.resolve({ip, port}));
    } catch (boost::system::system_error& e) {
        std::cout << "Failed to connect: " << e.what() << std::endl;
        return;
    }
    std::cout << "Connected successfully" << std::endl;
}

int ElevatorDriver::get_button_signal(int button, int floor) {
    std::lock_guard<std::mutex> lock(mtx);
    boost::asio::write(socket, boost::asio::buffer((char[4]) {5, button, floor}, 4));
    char response[4];
    boost::asio::read(socket, boost::asio::buffer(response, 4));
    return response[3];
}

int ElevatorDriver::get_floor_sensor_signal() {
    std::lock_guard<std::mutex> lock(mtx);
    boost::asio::write(socket, boost::asio::buffer((char[4]) {6}, 4));
    char response[4];
    boost::asio::read(socket, boost::asio::buffer(response, 4));
    return response[3];
}

int ElevatorDriver::get_stop_signal() {
    std::lock_guard<std::mutex> lock(mtx);
    boost::asio::write(socket, boost::asio::buffer((char[4]) {7}, 4));
    char response[4];
    boost::asio::read(socket, boost::asio::buffer(response, 4));
    return response[3];
}

int ElevatorDriver::get_obstruction_signal() {
    std::lock_guard<std::mutex> lock(mtx);
    boost::asio::write(socket, boost::asio::buffer((char[4]) {8}, 4));
    char response[4];
    boost::asio::read(socket, boost::asio::buffer(response, 4));
    return response[3];
}

