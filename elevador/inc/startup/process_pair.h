#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <thread>
#include <boost/thread.hpp>
#include <chrono>
#include <system_error> // For std::error_code
#include <cstdlib> // For system command

#include "config.h"

using boost::asio::ip::udp;
using namespace std::chrono_literals;

auto last_receive_time = std::chrono::steady_clock::now();

// Runs the primary
// Sends heartbeats to the backup
// Runs the primary dispatch logic
void run_primary(const std::string& host) { // unsused? , const std::string& port
    try {
        
        // Socket setup for primary
        boost::asio::io_service io_service;
        udp::resolver resolver(io_service);
        udp::endpoint receiver_endpoint = *resolver.resolve({udp::v4(), host, BACKUP_PORT});
        udp::socket socket(io_service);
        socket.open(udp::v4());
        socket.set_option(boost::asio::socket_base::reuse_address(true));

        // Launching backup from another window
        launch_backup();

        // launch primary thread
        launch_primary();

        // launch heartbeat thread
        while (true) {
            std::string message = "donk donk";
            socket.send_to(boost::asio::buffer(message), receiver_endpoint);
            boost::this_thread::sleep_for(boost::chrono::milliseconds(HEARTBEAT_RATE_MS));
        }

        socket.close();
    
    } 
    catch (std::exception& e) {
        std::cerr << "Primary Exception: " << e.what() << std::endl;
    }
}

// Runs the backup
// Checks for heartbeat from the primary thread
// If no heartbeats are found, it will begin as a primary thread and launch backup window
void run_backup(const std::string& host, const std::string& port) {
    try {
        
        // SOCKETS FOR BACKUP
        boost::asio::io_service io_service;
        udp::socket socket(io_service, udp::endpoint(udp::v4(), std::stoi(port)));
        socket.set_option(udp::socket::reuse_address(true));
        socket.non_blocking(true);
        udp::endpoint sender_endpoint;

        bool is_primary_now = false;
        char buf[1024];

        while (!is_primary_now) {
            std::cout << "Backup running" << std::endl;

            boost::system::error_code error;
            socket.receive_from(boost::asio::buffer(buf), sender_endpoint, 0, error); // "size_t length =" is not used so i removed it

            // Checks if the primary is still alive, if not 
            if (error == boost::asio::error::would_block) {
            
                if (!is_primary_now && std::chrono::steady_clock::now() - last_receive_time > std::chrono::milliseconds(HEARTBEAT_TIMEOUT_DURATION_MS)) {
                    is_primary_now = true;

                    break;
                }

                // sleep for 1 second
                boost::this_thread::sleep_for(boost::chrono::milliseconds(HEARTBEAT_RATE_MS));
                continue;
            }

            if (error && error != boost::asio::error::message_size) {
                    throw boost::system::system_error(error);
            }

            // Reset last_receive time
            last_receive_time = std::chrono::steady_clock::now();
        }

        socket.close();

        // Run the primary because you are now the primary :))
        run_primary(host); // , PRIMARY_PORT didnt use this any more
    } 

    catch (std::exception& e) {
        std::cerr << "Backup Exception: " << e.what() << std::endl;
    }
}