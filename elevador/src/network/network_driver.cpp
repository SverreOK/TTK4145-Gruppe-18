#include "network_driver.h"

using boost::asio::ip::udp;

Network_driver::Network_driver(Super_container* data_container) : data_container(data_container)
{
    status_broadcast_socket.open(udp::v4());
    status_broadcast_socket.set_option(udp::socket::reuse_address(true));
    status_broadcast_socket.set_option(udp::socket::broadcast(true));
    status_broadcast_socket.bind(udp::endpoint(udp::v4(), 12345));

    

    call_socket.open(udp::v4());
    call_socket.set_option(udp::socket::reuse_address(true));
    call_socket.set_option(udp::socket::broadcast(true));
    call_socket.bind(udp::endpoint(udp::v4(), 12346));
}

void Network_driver::infinite_status_broadcast()
{
    while (true)
    {
        elevator_status_network status = data_container->get_elevator_by_id(data_container->get_my_id())->get_status_network();
        status_broadcast_socket.send_to(boost::asio::buffer(&status, sizeof(status)), udp::endpoint(boost::asio::ip::address::from_string("255.255.255.255"), 12345));
        boost::this_thread::sleep_for(boost::chrono::milliseconds(10));
    }
}

void Network_driver::infinite_call_recv()
{
    char buffer[1024];
    boost::asio::ip::udp::endpoint sender_endpoint;
    while (true)
    {
        size_t len = call_socket.receive_from(boost::asio::buffer(buffer), sender_endpoint);
        call_message* incoming_call = (call_message*)buffer;
        //data_container->add_call(new Call(*incoming_call)); //this should also merge the call if it already exists
        bool retransmit = false;
        for (auto elevators : data_container->get_alive_elevators())
        {
            bool is_acked = false;
            for (auto acks : incoming_call->ack_list)
            {   
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
    }
}





Network_driver::~Network_driver()
{

}
