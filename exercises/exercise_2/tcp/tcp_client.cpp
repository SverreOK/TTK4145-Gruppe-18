#include <iostream>
#include <boost/asio.hpp>
#include <chrono>

using boost::asio::ip::tcp;

int main() {
    try {
        std::string TCP_Server = "10.100.23.186";
        unsigned short TCP_Port = 33546;

        boost::asio::io_service io_service;

        // Endpoint / server to connect to, which is labs TCP server with the \0 end port
        tcp::endpoint endpoint(boost::asio::ip::address::from_string(TCP_Server), TCP_Port);

        // Creating socket and connecting it to said port 
        std::cout << "Connecting..." << std::endl;
        tcp::socket socket(io_service);
        socket.connect(endpoint);
        std::cout << "Connected!" << std::endl;

        // find local ip adresses and port
        tcp::endpoint local_endpoint = socket.local_endpoint();
        std::string local_ip_address = local_endpoint.address().to_string();
        unsigned short local_port = local_endpoint.port();

        // confirmation of connection
        std::cout << "Connected to the server at: " << endpoint.address() << ":" << endpoint.port() << std::endl;

        // Receive the echoed message from the server
        char echoedMessage[256]; // Adjust the buffer size as needed
        socket.receive(boost::asio::buffer(echoedMessage));

        // Print the echoed message
        std::cout << "Received echoed message: " << echoedMessage << std::endl;       

    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        if (auto* se = dynamic_cast<boost::system::system_error*>(&e)) {
        std::cerr << "Boost error code: " << se->code() << std::endl;
        }
    }

    return 0;
}
