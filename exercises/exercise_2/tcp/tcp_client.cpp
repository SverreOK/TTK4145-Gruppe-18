#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main() {
    try {
        std::string TCP_Server = "10.100.23.129";
        unsigned short TCP_Port = 33546;

        boost::asio::io_service io_service;

        // Endpoint / server to connect to, which is labs TCP server with the \0 end port
        tcp::endpoint endpoint(boost::asio::ip::address::from_string(TCP_Server), TCP_Port);

        // Creating socket and connecting it to said port 
        tcp::socket socket(io_service);
        socket.connect(endpoint);

        // find local ip adresses and port
        tcp::endpoint local_endpoint = socket.local_endpoint();
        std::string local_ip_address = local_endpoint.address().to_string();
        unsigned short local_port = local_endpoint.port();

        // Message to connect as specified in readme and \0 at end
        std::string message = "Connect to: " + local_ip_address + ":" + std::to_string(local_port) + "\0";
        std::string message2 = "Whats up dawg \0";

        // confirmation of connection
        std::cout << "Connected to the server at: " << endpoint.address() << ":" << endpoint.port() << std::endl;

        // Send the message to the server
        socket.send(boost::asio::buffer(message));

        // Receive the echoed message from the server
        char echoedMessage[256]; // Adjust the buffer size as needed
        size_t bytesRead = socket.receive(boost::asio::buffer(echoedMessage));

        // Print the echoed message
        std::cout << "Received echoed message: " << echoedMessage << std::endl;

        socket.send(boost::asio::buffer(message2));
        char echoedMessage2[256]; // Adjust the buffer size as needed
        size_t bytesRead2 = socket.receive(boost::asio::buffer(echoedMessage));
        bytesRead = socket.receive(boost::asio::buffer(echoedMessage2));
        std::cout << echoedMessage2 << std::endl;

        // Close the socket
        socket.close();

        // Print confirmation of message sent
        std::cout << "Message sent successfully!" << std::endl;
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
