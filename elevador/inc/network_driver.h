#pragma once

#include <string>


class Network_driver
{
private:
    /* data */
public:
    Network_driver();
    ~Network_driver();
    void send(const std::string& ip, const std::string& port);
};

