#include "door_timer.h"

door_timer::door_timer(boost::asio::io_service& io) : timer(io), running(false) {}

door_timer::~door_timer() {
    timer.cancel();
}

void door_timer::start(std::function<void(elevator_event)> timeout_callback) {
    running = true;
    timer.expires_from_now(boost::posix_time::seconds(RESET_TIME));
    timer.async_wait([this, timeout_callback](const boost::system::error_code& error) {
        if (!error && running) {
            running = false;
            timeout_callback(elevator_event::DOOR_TIMEOUT);
        }
    });
}

void door_timer::reset(std::function<void(elevator_event)> timeout_callback) {
    timer.cancel(); // Cancel the existing async wait
    timer.expires_from_now(boost::posix_time::seconds(RESET_TIME));
    running = true;
    timer.async_wait([this, timeout_callback](const boost::system::error_code& error) {
        if (!error && running) {
            running = false;
            timeout_callback(elevator_event::DOOR_TIMEOUT);
        }
    });
}

bool door_timer::isRunning() {
    return running;
}