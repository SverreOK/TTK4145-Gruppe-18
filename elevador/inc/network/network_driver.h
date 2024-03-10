#pragma once

#include <string>
#include <array>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include "super_container.h"


class Network_driver
{
private:
    Super_container* data_container;
    boost::asio::io_service io_service;
    boost::asio::ip::udp::socket status_broadcast_socket{io_service};
    boost::asio::ip::udp::socket call_socket{io_service};

public:
    Network_driver(Super_container* data_container);
    ~Network_driver();
    void infinite_status_broadcast();
    void infinite_call_recv();
};

