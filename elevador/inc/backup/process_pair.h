#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <system_error> // For std::error_code
#include <cstdlib> // For system command

using boost::asio::ip::udp;
using namespace std::chrono_literals;

auto last_receive_time = std::chrono::steady_clock::now();

void launch_backup() {
    std::string command = "gnome-terminal -- bash -c './main backup; exec bash'";
    std::system(command.c_str());
}

void run_primary(const std::string& host, const std::string& port);  // Forward declaration

void run_backup(const std::string& host, const std::string& port) {
    try {
        
        // SOCKETS FOR BACKUP
        boost::asio::io_service io_service;
        udp::socket socket(io_service, udp::endpoint(udp::v4(), std::stoi(port)));
        socket.set_option(udp::socket::reuse_address(true));
        udp::endpoint sender_endpoint;

        // BUFFER THAT SHOULD CONTAIN THE BACKUP FOR THE PRIMARY DATA
        char buf[1024];


        int last_received_count = 0;
        bool is_primary_now = false;

        while (true) {
            std::cout << "Backup running" << std::endl;
            socket.non_blocking(true);
            boost::system::error_code error;
            size_t length = socket.receive_from(boost::asio::buffer(buf), sender_endpoint, 0, error);

            if (error == boost::asio::error::would_block) {
                // No data received yet, check for timeout
                if (!is_primary_now && last_received_count > 0 && std::chrono::steady_clock::now() - last_receive_time > 3s) {
                    // Assume primary has failed, take over as primary
                    std::cout << "Primary failure detected, taking over as primary." << std::endl;
                    is_primary_now = true;
                    //launch_process(); // Launch a new backup process
                    break; // Exit the loop to start running as primary
                }
                std::this_thread::sleep_for(1s); // Sleep to avoid busy waiting
                continue;
            }

            if (error && error != boost::asio::error::message_size) {
                throw boost::system::system_error(error);
            }

            last_received_count = std::atoi(std::string(buf, length).c_str());
            last_receive_time = std::chrono::steady_clock::now(); // Update last receive time
            std::cout << "Backup received count: " << last_received_count << std::endl;
        }
        socket.close();


        run_primary(host, "27107"); // Start running as primary, how do we give everything that primary needs?



    } catch (std::exception& e) {
        std::cerr << "Backup Exception: " << e.what() << std::endl;
    }
}

void run_primary(const std::string& host, const std::string& port) {
    try {
        
        // SOCKETS FOR PRIMARY
        boost::asio::io_service io_service;
        udp::resolver resolver(io_service);
        udp::endpoint receiver_endpoint = *resolver.resolve({udp::v4(), host, "27107"});
        udp::socket socket(io_service);
        socket.open(udp::v4());

        launch_backup(); // Launch a backup process

        // launch primary thread
        // launch heartbeat thread

        socket.close();


    
    } catch (std::exception& e) {
        std::cerr << "Primary Exception: " << e.what() << std::endl;
    }
}

void send_state() {
    //std::string serialized_state = serialize_state(data_container, event_queue);
    //socket.send_to(boost::asio::buffer(serialized_state), backup_endpoint);
}

void receive_state() {
    // socket.async_receive_from(
    //     boost::asio::buffer(recv_buffer), remote_endpoint,
    //     [this](boost::system::error_code ec, std::size_t bytes_recvd) {
    //         if (!ec && bytes_recvd > 0) {
    //             deserialize_state(recv_buffer, data_container, event_queue);
    //         }
    //     });
}