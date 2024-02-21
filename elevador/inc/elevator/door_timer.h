#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <functional>
#include "elevator/elevator_fsm.h"

#define RESET_TIME 3

class door_timer {
public:
    door_timer(boost::asio::io_service& io);
    ~door_timer();

    void start(std::function<void(elevator_event)> timeout_callback);
    void reset(std::function<void(elevator_event)> timeout_callback);
    bool isRunning();

private:
    boost::asio::deadline_timer timer;
    bool running;
};