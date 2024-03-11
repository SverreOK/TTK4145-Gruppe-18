#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/thread/concurrent_queues/sync_queue.hpp>
#include <iostream>
#include <string>
#include <jsoncpp/json/json.h>  

namespace asio = boost::asio;
using asio::ip::udp;


class Broadcast {

    private:
        boost::asio::io_context io_context;
        boost::asio::ip::udp::socket socket;
        unsigned short port;
        boost::asio::ip::udp::endpoint broadcast_endpoint;
    
    public:
        Broadcast(unsigned short port) : io_context(), socket(io_context, udp::endpoint(udp::v4(), port)), port(port) {
        socket.set_option(udp::socket::reuse_address(true));
        socket.set_option(asio::socket_base::broadcast(true));
        }

        /*
        ---------------------------------------------------------------------
        Receives and broadcasts data from the message queue
        Uses Boost.Serialization to serialize the data
        Serialized data is sent over UDP
        ---------------------------------------------------------------------
        */
        void sender(std::string message) {
            
        }
        /*
        ---------------------------------------------------------------------
        Receives broadcasted data
        Uses Boost.Serialization to deserialize the data
        Deserialized data is pushed to the message queue
        ---------------------------------------------------------------------
        */
        void receiver() {

        }
        
    private:
        void serialize() {
                
        }
        
        void deserialize() {
                
        }
};