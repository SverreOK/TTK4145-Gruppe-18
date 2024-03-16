#include "peer.h"

// CONSTRUCTOR
Peer::Peer(Super_container* data_container) : data_container(data_container) {
        broadcast_socket_tx.open(udp::v4());
        broadcast_socket_tx.set_option(udp::socket::reuse_address(true));
        broadcast_socket_tx.set_option(udp::socket::broadcast(true));
        broadcast_socket_tx.bind(udp::endpoint(udp::v4(), broadcast_tx_port)); // port is not imPORTant for tx

        broadcast_socket_rx.open(udp::v4());
        broadcast_socket_rx.set_option(udp::socket::reuse_address(true));
        broadcast_socket_rx.non_blocking(true);
        broadcast_socket_rx.bind(udp::endpoint(udp::v4(), broadcast_rx_port));

        call_socket_tx.open(udp::v4());
        call_socket_tx.set_option(udp::socket::reuse_address(true));
        call_socket_tx.set_option(udp::socket::broadcast(true));
        call_socket_tx.bind(udp::endpoint(udp::v4(), call_tx_port)); // port is not imPORTant for tx

        call_socket_rx.open(udp::v4());
        call_socket_rx.set_option(udp::socket::reuse_address(true));
        call_socket_rx.non_blocking(true);
        call_socket_rx.bind(udp::endpoint(udp::v4(), call_rx_port));

        broadcast_address = boost::asio::ip::address::from_string(BROADCAST_ADDRESS);
}

// Broadcast elevator status to whole network
void Peer::infinite_status_broadcast() {

    while (true) {

        if (data_container->get_elevator_by_id(data_container->get_my_id())->get_obstruction_status() == false){
            elevator_status_network status = data_container->get_elevator_by_id(data_container->get_my_id())->get_status_network();
            broadcast_socket_tx.send_to(boost::asio::buffer(&status, sizeof(status)), udp::endpoint(broadcast_address, 12345));
            boost::this_thread::sleep_for(boost::chrono::milliseconds(BROADCAST_RATE_MS));
        }

        boost::this_thread::sleep_for(boost::chrono::milliseconds( static_cast<int>(BROADCAST_RATE_MS/4)));
        //std::cout << "Broadcasting status" << std::endl;
    }   
}

// Receives status from other elevators and adds them to the list of alive elevators
void Peer::infinite_status_recieve() {

    char buffer[1024];
    boost::asio::ip::udp::endpoint sender_endpoint;
    while (true) {
        try
        {
            broadcast_socket_rx.receive_from(boost::asio::buffer(buffer), sender_endpoint); //len not used :grinning: : size_t len = 
        }
        catch(const std::exception& e)
        {
            boost::this_thread::sleep_for(boost::chrono::milliseconds(STATUS_RECEIVE_RATE_MS));
            continue;
        }
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

// Removes connections after they have been dead for 5 seconds
void Peer::dead_connection_remover() {
    while (true) {
        for (auto id : data_container->get_alive_elevators()) {
            if (data_container->get_elevator_by_id(id)->get_last_seen() < time(NULL) - DEAD_CONNECTION_REMOVAL_TIME_S && strncmp(id.id.c_str(), my_id.id.c_str(), 8) != 0) {
                data_container->remove_elevator(id);
            }
        }
    }
}



/*
 * This function is responsible for continuously receiving calls from other peers.
 * It runs an infinite loop that listens for incoming data on the socket.
 * For each received call, it checks if an identical call already exists in the data container.
 * If the call does not exist, it checks if the call needs to be retransmitted.
 * A call needs to be retransmitted if there are any alive elevators that have not acknowledged the call,
 * or if the call has been serviced by some elevators but not all.
 * The function then acknowledges the call and adds it to the data container.
 * If the call is marked for retransmission, it transmits the call.
 * After processing the call, the function sleeps for 32 milliseconds before starting the next iteration.
 */
void Peer::infinite_call_recieve() {
        char buffer[1024];
        boost::asio::ip::udp::endpoint sender_endpoint;
        while (true) {
            
            try
            {
                call_socket_rx.receive_from(boost::asio::buffer(buffer), sender_endpoint); // len not used :grinning: : size_t len = 
            }
            catch(const std::exception& e)
            {
                // std::cout << "emty buffer" << std::endl;
                boost::this_thread::sleep_for(boost::chrono::milliseconds(CALL_RECEIVE_RATE_MS));
                continue;
            }
            
            call_message* incoming_call = (call_message*)buffer;

            Call* new_call = new Call(*incoming_call);
            
            bool already_exists = false;
            //check if an IDENTICAL call, with same id, ack list and serviced list already exists
            
            for (auto call : data_container->get_call_list()) {
                if (call->get_call_id()->call_number == new_call->get_call_id()->call_number &&
                    call->get_call_id()->elevator_id.id == new_call->get_call_id()->elevator_id.id) {

                        bool sameacklist = vectors_are_equal(call->get_elevator_ack_list(), new_call->get_elevator_ack_list());
                        bool sameservicedlist = vectors_are_equal(call->get_serviced_ack_list(), new_call->get_serviced_ack_list());

                        if (sameacklist &&
                            sameservicedlist) {
                                already_exists = true;
                                break;
                        }
                    }
                }

            if (!already_exists) {

                bool retransmit = false;

                std::vector<Elevator_id> new_call_ack_list = new_call->get_elevator_ack_list();
                std::vector<Elevator_id> new_call_serviced_list = new_call->get_serviced_ack_list();
                std::vector<Elevator_id> alive_elevators = data_container->get_alive_elevators();

                if (!vector_elements_in_A_found_in_B(alive_elevators, new_call_ack_list) ||
                    (!vector_elements_in_A_found_in_B(alive_elevators, new_call_serviced_list) && new_call_serviced_list.size() > 0)) {
                    retransmit = true;
                }
                
                new_call->acknowlegde_call(my_id); //if the id is already in the ack list, it will not be added

                if( new_call->get_serviced_ack_list().size() > 0){ //if the call has already been serviced by some elevator, ack
                    new_call->service_call(my_id);
                }

                Call* call_in_data = data_container->add_call(new_call);
                
                if (retransmit)
                {
                    call_transmit(call_in_data, 1);
                }
            }
        }
}

/*
 * This function is responsible for continuously transmitting calls to elevators.
 * It runs an infinite loop that iterates over all the calls in the data container.
 * For each call, it retrieves the list of elevators that have acknowledged the call,
 * the list of elevators that have serviced the call, and the list of currently alive elevators.
 * If there are any elevators in the alive list that have not acknowledged or serviced the call,
 * it transmits the call to those elevators.
 * After processing all calls, the function sleeps for 100 milliseconds before starting the next iteration.
 */ 

void Peer::infinite_call_transmit() {
    
    while (true) {
        for (auto call : data_container->get_call_list()) {

            std::vector<Elevator_id> call_ack_list = call->get_elevator_ack_list();
            std::vector<Elevator_id> call_serviced_list = call->get_serviced_ack_list();
            std::vector<Elevator_id> alive_elevators = data_container->get_alive_elevators();
            
            if(!vector_elements_in_A_found_in_B(alive_elevators, call_ack_list) ||
               (!vector_elements_in_A_found_in_B(alive_elevators, call_serviced_list) && call_serviced_list.size() > 0)){
                call_transmit(call, 1);
            }
        }
        //sleep
        boost::this_thread::sleep_for(boost::chrono::milliseconds(CALL_TRANSMIT_RATE_MS));
    }
}

void Peer::call_transmit(Call* call, int burst_size) {
    for (int i = 0; i < burst_size; i++) {
        // make the call into a call_message
        call_message call_msg = call->get_call_message();
        call_socket_tx.send_to(boost::asio::buffer(&call_msg, sizeof(call_message)), udp::endpoint(broadcast_address, call_rx_port));
        //call_socket_tx.send_to(boost::asio::buffer(call_msg, sizeof(call_message)), udp::endpoint(broadcast_address, call_rx_port));
    }
}

/*
* Runs the peer threads
*/

void purge_receive_buffers(boost::asio::ip::udp::socket& call_socket_rx) {
    try {
        while (1){
            char buffer[1024];
            boost::asio::ip::udp::endpoint sender_endpoint;
            call_socket_rx.receive_from(boost::asio::buffer(buffer), sender_endpoint); // size_t len =
            boost::this_thread::interruption_point();
        }
    } catch (std::exception& e) {
        // Thread was interrupted, handle it if necessary
        std::cout << "I've been excepting you, Mr. Bond" << std::endl;
    }
}


void Peer::run_peer() {

    boost::thread purge_thread(purge_receive_buffers, std::ref(call_socket_rx));
    boost::this_thread::sleep_for(boost::chrono::milliseconds(SOCKET_CLEAR_RATE_MS));
    purge_thread.interrupt();
    purge_thread.join();
    std::cout << "Purged receive buffers :grinning:" << std::endl;

    boost::thread peer_thread(&Peer::infinite_status_broadcast, this);
    boost::thread peer_thread2(&Peer::infinite_status_recieve, this);
    boost::thread peer_thread3(&Peer::dead_connection_remover, this);
    boost::thread peer_thread4(&Peer::infinite_call_recieve, this);
    boost::thread peer_thread5(&Peer::infinite_call_transmit, this);
}