#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

class PeerDiscovery {
public:
    struct PeerUpdate {
        std::vector<std::string> peers;
        std::string New;
        std::vector<std::string> lost;
    };

private:
    boost::asio::io_context io_context;
    boost::asio::ip::udp::socket socket_;
    boost::asio::ip::udp::endpoint broadcast_endpoint_;
    std::map<std::string, boost::posix_time::ptime> lastSeen;
    const std::chrono::milliseconds interval{15};
    const std::chrono::milliseconds timeout{500};
    std::string id;
    int port;


public:

    /*
    ---------------------------------------------------------------------
    Broadcasts the elevator's id to the network. Can be toggled.

    ---------------------------------------------------------------------
    */
    void start_transmitter() {

    }

    /*
    ---------------------------------------------------------------------
    Finds new peers and lost peers.
    Removes lost peers after a timeout.
    Sending peer updates to the message queue.
    ---------------------------------------------------------------------
    
    */
    void start_receiver() {

    }

private:
    void send() {

    }

    void receive() {

    }

    void receive_id() {

    }

};