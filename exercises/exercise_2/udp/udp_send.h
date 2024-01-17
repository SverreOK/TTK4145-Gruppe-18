// udp_sender.h
#include <boost/asio.hpp>
#include <string>

using boost::asio::ip::udp;

class Udp_Sender {
public:
    Udp_Sender(const std::string& ip, const std::string& port);
    ~Udp_Sender();
    void send(const std::string& message);

private:
    boost::asio::io_context io_context;
    udp::socket socket;
    udp::endpoint endpoint;
};