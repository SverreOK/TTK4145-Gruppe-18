#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <functional>

#define RESET_TIME 3

class DoorTimer {
public:
    DoorTimer(boost::asio::io_service& io);
    ~DoorTimer();

    void start(std::function<void()> timeoutCallback);
    void reset();
    bool isRunning();

private:
    boost::asio::deadline_timer timer;
    bool running;
};