#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

// udp_sender.cpp
#include "udp_send.h"

Udp_Sender::Udp_Sender(const std::string& ip, const std::string& port)
    : socket(io_context) {
    udp::resolver resolver(io_context);
    endpoint = *resolver.resolve(udp::v4(), ip, port).begin();
    socket.open(udp::v4());
}
Udp_Sender::~Udp_Sender() {
    socket.close();
}

void Udp_Sender::send(const std::string& message) {
    socket.send_to(boost::asio::buffer(message), endpoint);
}



// #define IPAddr "10.100.23.29" //local ip
#define IPAddr "0.0.0.0" //local ip
#define PORT "20018"          //port

int main(){

    //boost wait timer
    boost::asio::io_context io_context;
    boost::asio::steady_timer timer(io_context, boost::asio::chrono::seconds(1));


    Udp_Sender sender(IPAddr, PORT);
    for (int i = 0; i < 10; i++)
    {
        sender.send("Message nr. " + std::to_string(i));
        std::cout << "Message nr. " + std::to_string(i) + " has been sent"<< std::endl;
        //use time so message is sent every second
        timer.wait();
        timer.expires_after(boost::asio::chrono::seconds(1));
    }
    
}