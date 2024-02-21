#include "inc/elevator/door_timer.h"

door_timer::door_timer(boost::asio::io_service& io) : timer(io), running(false) {}

door_timer::~door_timer() {
    timer.cancel();
}

void door_timer::start(std::function<void()> timeoutCallback) {
    running = true;
    timer.expires_from_now(boost::posix_time::seconds(RESET_TIME));
    timer.async_wait([this, timeoutCallback](const boost::system::error_code& error) {
        if (!error && running) {
            running = false;
            timeoutCallback();
        }
    });
}

void door_timer::reset() {
    running = true;
    timer.expires_from_now(boost::posix_time::seconds(RESET_TIME));
}

bool door_timer::isRunning() {
    return running;
}