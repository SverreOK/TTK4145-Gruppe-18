#include "inc/elevator/door_timer.h"

DoorTimer::DoorTimer(boost::asio::io_service& io) : timer(io), running(false) {}

DoorTimer::~DoorTimer() {
    timer.cancel();
}

void DoorTimer::start(std::function<void()> timeoutCallback) {
    running = true;
    timer.expires_from_now(boost::posix_time::seconds(RESET_TIME));
    timer.async_wait([this, timeoutCallback](const boost::system::error_code& error) {
        if (!error && running) {
            running = false;
            timeoutCallback();
        }
    });
}

void DoorTimer::reset() {
    running = true;
    timer.expires_from_now(boost::posix_time::seconds(RESET_TIME));
}

bool DoorTimer::isRunning() {
    return running;
}