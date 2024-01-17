#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <iostream>

#define UDP_PORT 20018

using boost::asio::ip::udp;
using boost::asio::ip::address;

struct listener {
    boost::asio::io_service io_service;
    udp::socket socket{io_service};
    boost::array<char, 1024> recv_buffer;
    udp::endpoint remote_endpoint;

    // Initialize the socket
    listener() : socket(io_service, udp::endpoint(udp::v4(), UDP_PORT)) {
        // Set the socket option to allow broadcasting
        socket.set_option(boost::asio::socket_base::broadcast(true));
        wait();
    }

    void handle_receive(const boost::system::error_code& error, size_t bytes_transferred) {
        if (error) {
            std::cout << "Receive failed: " << error.message() << "\n";
            return;
        }
        std::cout << "Received: '" << std::string(recv_buffer.begin(), recv_buffer.begin() + bytes_transferred) << "'\n";
        wait();  // Continue waiting for the next incoming message
    }

    void wait() {
        socket.async_receive_from(boost::asio::buffer(recv_buffer),
                                  remote_endpoint,
                                  boost::bind(&listener::handle_receive, this,
                                              boost::asio::placeholders::error,
                                              boost::asio::placeholders::bytes_transferred));
    }

    void Receiver() {
        std::cout << "From port: " << UDP_PORT << "\n";
        std::cout << "Remote endpoint / address: " << remote_endpoint.address() << "\n";
        io_service.run();
        std::cout << "Receiver exit\n";
    }

    ~listener() {
        socket.close();
    }
};

int main() {
    listener listener;

    std::thread r([&] { listener.Receiver(); });

    r.join();
}
