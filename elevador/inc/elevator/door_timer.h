#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <functional>

#define RESET_TIME 3

class door_timer {
public:
    door_timer(boost::asio::io_service& io);
    ~door_timer();

    void start(std::function<void()> timeoutCallback);
    void reset();
    bool isRunning();

private:
    boost::asio::deadline_timer timer;
    bool running;
};