// //UDP sending function

// #include <iostream>
// #include <string>
// #include <boost/asio.hpp>

// using boost::asio::ip::udp;

// void udp_send(const std::string& message, int port, const std::string& ip_address) {
//     try {
//         boost::asio::io_service io_service;

//         udp::socket socket(io_service);
//         socket.open(udp::v4());

//         udp::endpoint remote_endpoint = udp::endpoint(boost::asio::ip::address::from_string(ip_address), port);

//         socket.send_to(boost::asio::buffer(message), remote_endpoint);
//     } catch (std::exception& e) {
//         std::cerr << e.what() << std::endl;
//     }
// }

#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

int main_my(int argc, char* argv[]){
    try
    {
        if (argc != 2){
            
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}
