#include "inc/elevator/door_timer.h"

door_timer::door_timer() : timer(io, boost::posix_time::seconds(RESET_TIME)) {}

void door_timer::start() {
    timer.async_wait(boost::bind(&door_timer::reset, this));
    io.run();
}

void door_timer::reset() {
    timer.expires_from_now(boost::posix_time::seconds(RESET_TIME));
}

bool door_timer::is_running() {
    return timer.expires_from_now().is_negative();
}