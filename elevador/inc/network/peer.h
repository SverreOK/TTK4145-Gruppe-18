#pragma once

#include <string>
#include <array>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include "super_container.h"
#include "config.h"

using boost::asio::ip::udp;

class Peer
{
private:
    Super_container* data_container;
    boost::asio::io_service io_service;
    boost::asio::ip::udp::socket broadcast_socket_tx{io_service};
    boost::asio::ip::udp::socket broadcast_socket_rx{io_service};
    boost::asio::ip::udp::socket call_socket_tx{io_service};
    boost::asio::ip::udp::socket call_socket_rx{io_service};

    boost::asio::ip::address broadcast_address;

    Elevator_id my_id = {ELEVATOR_ID};

    int broadcast_tx_port = BROADCAST_TX_PORT;
    int broadcast_rx_port = BROADCAST_RX_PORT;
    int call_tx_port = CALL_TX_PORT;
    int call_rx_port = CALL_RX_PORT;

    boost::mutex mtx;

public:
    Peer(Super_container* data_container);

    ~Peer();

    // Background threads
    void infinite_status_broadcast();
    void infinite_status_recieve();
    void infinite_call_recieve();
    void infinite_call_transmit();
    void dead_connection_remover();
    
    void call_transmit(Call* call, int burst_size);
    void run_peer();
};
