#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <vector>
#include <iostream>
#include <string>

// Startup logic contains all necessary includes
#include "start_logic.h"
#include "process_pair.h"

int main(int argc, char* argv[]) {

    bool is_backup = argc > 1 && std::string(argv[1]) == "backup";

    if (is_backup && BACKUP_TOGGLE) {

        // Running backup before starting primary loop
        std::cout << "Running backup!" << std::endl;
        run_backup(HEARTBEAT_ADDRESS, BACKUP_PORT);
        
    }
    else if (BACKUP_TOGGLE) {

        // Running primary loop
        // Starts the program together with heartbeat
        std::cout << "Running primary!" << std::endl;
        run_primary(HEARTBEAT_ADDRESS, PRIMARY_PORT);

    }
    else {

        // Start as normal if backup is not enabled
        std::cout << "Backup is not enabled" << std::endl;
        launch_with_no_backup();
    }
    return 0;
}
