#include <string>
#include "process_pair.h"   

int main(int argc, char* argv[]) {
    std::string host = "127.0.0.1";
    std::string port = "27106";
    std::string backup_port = "27107";
    bool isBackup = argc > 1 && std::string(argv[1]) == "backup";

    if (isBackup) {
        runBackup(host, backup_port);
    } else {
        runPrimary(host, port, 0);
    }

    return 0;
}