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

    boost::asio::ip::address broadcast_address;

    Elevator_id my_id = {ELEVATOR_ID};

    int tx_port = 9879;
    int rx_port = 12345;


public:
    Peer(Super_container* data_container) : data_container(data_container) {
        broadcast_socket_tx.open(udp::v4());
        broadcast_socket_tx.set_option(udp::socket::reuse_address(true));
        broadcast_socket_tx.set_option(udp::socket::broadcast(true));
        broadcast_socket_tx.bind(udp::endpoint(udp::v4(), tx_port)); // port is not imPORTant for tx

        broadcast_socket_rx.open(udp::v4());
        broadcast_socket_rx.set_option(udp::socket::reuse_address(true));
        broadcast_socket_rx.bind(udp::endpoint(udp::v4(), rx_port));

        broadcast_address = boost::asio::ip::address::from_string("255.255.255.255");
    }

    ~Peer();


    void infinite_status_broadcast() {

        while (true) {
            elevator_status_network status = data_container->get_elevator_by_id(data_container->get_my_id())->get_status_network();
            broadcast_socket_tx.send_to(boost::asio::buffer(&status, sizeof(status)), udp::endpoint(broadcast_address, 12345));
            boost::this_thread::sleep_for(boost::chrono::milliseconds(1000));
            std::cout << "Broadcasting status" << std::endl;
        }   
    }

    void infinite_status_recieve() {

        char buffer[1024];
        boost::asio::ip::udp::endpoint sender_endpoint;

        while (true) {

            size_t len = broadcast_socket_rx.receive_from(boost::asio::buffer(buffer), sender_endpoint);
            elevator_status_network* incoming_status = (elevator_status_network*)buffer;
            Elevator_state* recv_elev = new Elevator_state(*incoming_status);
            int should_delete = 0;

            std::cout << "Received elevator with id: " << recv_elev->get_id().id << std::endl;

            if (strncmp(recv_elev->get_id().id.c_str(), data_container->get_my_id().id.c_str(), 8) != 0){
                should_delete = data_container->add_elevator(recv_elev); // delete if already exists, else add to elevator list
                std::cout << "Deleted elevator with id: " << recv_elev->get_id().id << std::endl;

            }
            if (should_delete){delete recv_elev;}
        }
    }

    void dead_connection_remover() {
        while (true) {
            for (auto id : data_container->get_alive_elevators()) {
                if (data_container->get_elevator_by_id(id)->get_last_seen() < time(NULL) - 5 && strncmp(id.id.c_str(), my_id.id.c_str(), 8) != 0){
                    data_container->remove_elevator(id);
                    std::cout << "Removed elevator with id: " << id.id << std::endl;
                }
            }
            boost::this_thread::sleep_for(boost::chrono::milliseconds(1000));
        }
    }
};
