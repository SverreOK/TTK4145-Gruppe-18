#include "peer.h"
#include "super_container.h"
#include "config.h"

Peer::Peer(Super_container* data_container) : data_container(data_container) {
        broadcast_socket_tx.open(udp::v4());
        broadcast_socket_tx.set_option(udp::socket::reuse_address(true));
        broadcast_socket_tx.set_option(udp::socket::broadcast(true));
        broadcast_socket_tx.bind(udp::endpoint(udp::v4(), broadcast_tx_port)); // port is not imPORTant for tx

        broadcast_socket_rx.open(udp::v4());
        broadcast_socket_rx.set_option(udp::socket::reuse_address(true));
        broadcast_socket_rx.bind(udp::endpoint(udp::v4(), broadcast_rx_port));

        call_socket_tx.open(udp::v4());
        call_socket_tx.set_option(udp::socket::reuse_address(true));
        call_socket_tx.set_option(udp::socket::broadcast(true));
        call_socket_tx.bind(udp::endpoint(udp::v4(), call_tx_port)); // port is not imPORTant for tx

        call_socket_rx.open(udp::v4());
        call_socket_rx.set_option(udp::socket::reuse_address(true));
        call_socket_rx.bind(udp::endpoint(udp::v4(), call_rx_port));

        broadcast_address = boost::asio::ip::address::from_string("255.255.255.255");
}

void Peer::infinite_status_broadcast() {

    while (true) {
        elevator_status_network status = data_container->get_elevator_by_id(data_container->get_my_id())->get_status_network();
        broadcast_socket_tx.send_to(boost::asio::buffer(&status, sizeof(status)), udp::endpoint(broadcast_address, 12345));
        boost::this_thread::sleep_for(boost::chrono::milliseconds(1000));
        //std::cout << "Broadcasting status" << std::endl;
        for (auto id : data_container->get_alive_elevators()) {
            std::cout << "Alive elevator: " << id.id << std::endl;
        }
        std::cout << "--------------------------------------" << std::endl;
    }   
}

void Peer::infinite_status_recieve() {

    char buffer[1024];
    boost::asio::ip::udp::endpoint sender_endpoint;
        while (true) {

        size_t len = broadcast_socket_rx.receive_from(boost::asio::buffer(buffer), sender_endpoint);
        elevator_status_network* incoming_status = (elevator_status_network*)buffer;
        Elevator_state* recv_elev = new Elevator_state(*incoming_status);
        //std::cout << "current floor remote elev: " << recv_elev->get_current_floor() << std::endl;
        int should_delete = 0;
        //std::cout << "Received elevator with id: " << recv_elev->get_id().id << std::endl;

        // we love C thats why we did C-strings here
        if (strncmp(recv_elev->get_id().id.c_str(), data_container->get_my_id().id.c_str(), 8)) {
            boost::lock_guard<boost::mutex> lock(mtx);
            should_delete = data_container->add_elevator(recv_elev); // delete if already exists, else add to elevator list
        }
        if (should_delete){delete recv_elev;}
    }
}

void Peer::dead_connection_remover() {
    while (true) {
        for (auto id : data_container->get_alive_elevators()) {
            if (data_container->get_elevator_by_id(id)->get_last_seen() < time(NULL) - 5 && strncmp(id.id.c_str(), my_id.id.c_str(), 8) != 0) {
                data_container->remove_elevator(id);
                std::cout << "Removed elevator with id: " << id.id << std::endl;
            }
        }
    }
}

void Peer::infinite_call_recieve() {
    char buffer[1024];
    boost::asio::ip::udp::endpoint sender_endpoint;
    while (true) {

        size_t len = call_socket_rx.receive_from(boost::asio::buffer(buffer), sender_endpoint);
        call_message* incoming_call = (call_message*)buffer;
        //data_container->add_call(new Call(*incoming_call)); //this should also merge the call if it already exists
        bool retransmit = false;
        for (auto elevators : data_container->get_alive_elevators()) { // check if the call has been acked by all elevators

            bool is_acked = false;
            for (auto acks : incoming_call->ack_list) {

                char elev_id[8];
                strncpy(elev_id, elevators.id.c_str(), 8);
                if (strncmp(acks, elev_id, 8) == 0)
                {
                    is_acked = true;
                    break;
                }
            }

            if (!is_acked)
            {
                retransmit = true;
                break;
            }
        }
        if (retransmit)
        {
            //Check if own id is in the ack list, if not, add it and retransmit
            bool is_acked = false;
            for (auto acks : incoming_call->ack_list) {
                if (strncmp(acks, my_id.id.c_str(), 8) == 0)
                {
                    is_acked = true;
                    break;
                }
            }
            if (!is_acked)
            {
                for (auto acks : incoming_call->ack_list) {
                    bool all_null = true;
                    for (int i = 0; i < 8; i++) {
                        if (acks[i] != 0) {
                            all_null = false;
                            break;
                        }
                    }
                    if (all_null) {
                        strncpy(acks, my_id.id.c_str(), 8);
                        break;
                    }
                }
            }
            call_transmit(new Call(*incoming_call), 10);
        }
    }
}

/*
Shall transfer all calls from the call list that has not been acked by all elevators yet

*/
void Peer::infinite_call_transmit() {
    
    while (true) {
        for (auto call : data_container->get_call_list()) {
            
            for (auto elevators : data_container->get_alive_elevators()) { // check if the call has been acked by all elevators

                bool is_acked = false;
                for (auto acks : call->get_elevator_ack_list()) {
                    char elev_id[8];
                    strncpy(elev_id, elevators.id.c_str(), 8);
                    if (strncmp(acks.id.c_str(), elev_id, 8) == 0) {
                        is_acked = true;
                        break;
                    }
                }

                if (!is_acked)
                {
                    call_transmit(call, 1);
                    break;
                }
            }
        }
    }
}

void Peer::call_transmit(Call* call, int burst_size) {
    for (int i = 0; i < burst_size; i++) {
        call_socket_tx.send_to(boost::asio::buffer(call, sizeof(Call)), udp::endpoint(broadcast_address, call_tx_port));
    }
}