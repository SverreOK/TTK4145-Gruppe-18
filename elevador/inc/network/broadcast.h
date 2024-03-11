#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <iostream>
#include <string>
#include <jsoncpp/json/json.h>  

namespace asio = boost::asio;
using asio::ip::udp;


class Broadcast {

    private:
        boost::asio::io_context transmitter_io_context;
        boost::asio::io_context receiver_io_context;
        boost::asio::ip::udp::socket transmitter_socket;
        boost::asio::ip::udp::socket receiver_socket;
        unsigned short port;
        boost::asio::ip::udp::endpoint broadcast_endpoint;
    
    public:
        Broadcast(unsigned short port) : 
        transmitter_io_context(), transmitter_socket(transmitter_io_context, udp::endpoint(udp::v4(), port)), port(port),
        receiver_io_context(), receiver_socket(receiver_io_context, udp::endpoint(udp::v4(), port)){


        transmitter_socket.set_option(udp::socket::reuse_address(true));
        transmitter_socket.set_option(asio::socket_base::broadcast(true));

        receiver_socket.set_option(udp::socket::reuse_address(true));
        receiver_socket.set_option(asio::socket_base::broadcast(true));

        // Set the broadcast endpoint to local network
        broadcast_endpoint = udp::endpoint(udp::v4(), port);
        }

        /*
        ---------------------------------------------------------------------
        Broadcasts arbitrary message to the network with a specified burst size
        ---------------------------------------------------------------------
        */
        void send(const std::string message, int burst_size) {

            boost::system::error_code error;

            // JSON sending
            Json::Value root;
            root["message"] = message;
            std::string json_message = root.toStyledString();
            Json::StreamWriterBuilder builder;
            const std::string serialized = Json::writeString(builder, root);

            for (size_t i = 0; i < burst_size; i++) {
                if (!error) {
                    transmitter_socket.send_to(asio::buffer(serialized), broadcast_endpoint);
                }
                else {
                    std::cerr << "Error: " << error.message() << std::endl;
                }
            }
        }
        /*
        ---------------------------------------------------------------------
        Receives broadcasted data from the network
        ---------------------------------------------------------------------
        */
        void receiver() {
            char data[1024];
            boost::system::error_code error;

            while (true) {
                size_t len = receiver_socket.receive_from(boost::asio::buffer(data), broadcast_endpoint, 0, error);
                if (error && error != boost::asio::error::message_size) {
                    std::cerr << "Receive failed: " << error.message() << std::endl;
                    continue;
                }

                std::string message(data, len);
                std::cout << "Received: " << message << std::endl;

                Json::Value root;
                Json::Reader reader;
                bool parsingSuccessful = reader.parse(message, root);
                if (!parsingSuccessful) {
                    std::cerr << "Failed to parse the received message as JSON: " << reader.getFormattedErrorMessages() << std::endl;
                    continue;
                }

            // Implement functionality to send the message to some other thread?
        }

        }
};