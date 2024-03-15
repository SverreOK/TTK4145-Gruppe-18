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

// Launches new terminal window with starting as backup
void launch_backup() {
    std::string command = "gnome-terminal -- bash -c './main backup; exec bash'";
    std::system(command.c_str());
}

void launch_primary() {
    std::cout << "Launching primary" << std::endl;
}

// Forward declaration
void run_primary(const std::string& host, const std::string& port); 

// Runs the backup
// Checks for heartbeat from the primary thread
// If no heartbeats are found, it will begin as a primary thread and launch backup window
void run_backup(const std::string& host, const std::string& port) {
    try {
        
        // SOCKETS FOR BACKUP
        boost::asio::io_service io_service;
        udp::socket socket(io_service, udp::endpoint(udp::v4(), std::stoi(port)));
        socket.set_option(udp::socket::reuse_address(true));
        udp::endpoint sender_endpoint;

        bool is_primary_now = false;
        char buf[1024];

        while (true) {
            std::cout << "Backup running" << std::endl;

            boost::system::error_code error;
            size_t length = socket.receive_from(boost::asio::buffer(buf), sender_endpoint, 0, error);

            // Checks if the primary is still alive, if not 
            if (!is_primary_now && std::chrono::steady_clock::now() - last_receive_time > 3s) {
                is_primary_now = true;
                run_primary(host, PRIMARY_PORT);
                break;
            }

            if (error && error != boost::asio::error::message_size) {
                throw boost::system::system_error(error);
            }

            // Reset last_receive time
            last_receive_time = std::chrono::steady_clock::now();
        }

        socket.close();

        run_primary(host, PRIMARY_PORT); // Start running as primary, how do we give everything that primary needs?
    } 

    catch (std::exception& e) {
        std::cerr << "Backup Exception: " << e.what() << std::endl;
    }
}

// Runs the primary
// Sends heartbeats to the backup
// Runs the primary dispatch logic
void run_primary(const std::string& host, const std::string& port) {
    try {
        
        // Socket setup for primary
        boost::asio::io_service io_service;
        udp::resolver resolver(io_service);
        udp::endpoint receiver_endpoint = *resolver.resolve({udp::v4(), host, BACKUP_PORT});
        udp::socket socket(io_service);
        socket.open(udp::v4());

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