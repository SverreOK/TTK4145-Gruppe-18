#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <thread>
#include <boost/thread.hpp>
#include <chrono>
#include <system_error> // For std::error_code
#include <cstdlib> // For system command

#include "config.h"
#include "start_logic.h"

using boost::asio::ip::udp;
using namespace std::chrono_literals;

// Runs the primary
// Sends heartbeats to the backup
// Runs the primary dispatch logic
void run_primary(const std::string& host);

// Runs the backup
// Checks for heartbeat from the primary thread
// If no heartbeats are found, it will begin as a primary thread and launch backup window
void run_backup(const std::string& host, const std::string& port);