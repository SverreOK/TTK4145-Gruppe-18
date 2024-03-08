#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <system_error> // For std::error_code
#include <cstdlib> // For system command

using boost::asio::ip::udp;
using namespace std::chrono_literals;

auto lastReceiveTime = std::chrono::steady_clock::now();

int global_count;

void launch_process() {
    std::string command = "gnome-terminal -- bash -c './main backup; exec bash'";
    std::system(command.c_str());
}

void runPrimary(const std::string& host, const std::string& port, int count_from_prev);  // Forward declaration

void runBackup(const std::string& host, const std::string& port) {
    try {
        boost::asio::io_service io_service;
        udp::socket socket(io_service, udp::endpoint(udp::v4(), std::stoi(port)));
        socket.set_option(udp::socket::reuse_address(true));
        udp::endpoint sender_endpoint;
        char buf[1024];
        int lastReceivedCount = 0;
        bool isPrimaryNow = false;

        while (true) {
            std::cout << "Backup running" << std::endl;
            socket.non_blocking(true);
            boost::system::error_code error;
            size_t length = socket.receive_from(boost::asio::buffer(buf), sender_endpoint, 0, error);

            if (error == boost::asio::error::would_block) {
                // No data received yet, check for timeout
                if (!isPrimaryNow && lastReceivedCount > 5 && std::chrono::steady_clock::now() - lastReceiveTime > 5s) {
                    // Assume primary has failed, take over as primary
                    std::cout << "Primary failure detected, taking over as primary." << std::endl;
                    isPrimaryNow = true;
                    //launch_process(); // Launch a new backup process
                    break; // Exit the loop to start running as primary
                }
                std::this_thread::sleep_for(1s); // Sleep to avoid busy waiting
                continue;
            }

            if (error && error != boost::asio::error::message_size) {
                throw boost::system::system_error(error);
            }

            lastReceivedCount = std::atoi(std::string(buf, length).c_str());
            lastReceiveTime = std::chrono::steady_clock::now(); // Update last receive time
            std::cout << "Backup received count: " << lastReceivedCount << std::endl;
        }
        socket.close();
        runPrimary(host, "27107", lastReceivedCount); // Start running as primary
    } catch (std::exception& e) {
        std::cerr << "Backup Exception: " << e.what() << std::endl;
    }
}

void runPrimary(const std::string& host, const std::string& port, int count_from_prev) {
    try {
        if (count_from_prev > 0) {
            global_count = count_from_prev;
        }
        
        boost::asio::io_service io_service;
        udp::resolver resolver(io_service);
        udp::endpoint receiver_endpoint = *resolver.resolve({udp::v4(), host, "27107"});

        udp::socket socket(io_service);

        socket.open(udp::v4());

        launch_process(); // Launch a backup process

        while (true) {
            global_count++; // Increment the global count
            std::string message = std::to_string(global_count);
            socket.send_to(boost::asio::buffer(message), receiver_endpoint);
            std::cout << "Global count " << global_count << " sent." << std::endl;
            std::this_thread::sleep_for(1s); // Simulate some work and time passing between sends
        }
        socket.close();
    } catch (std::exception& e) {
        std::cerr << "Primary Exception: " << e.what() << std::endl;
    }
}
