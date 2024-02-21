#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#define RESET_TIME 3

class door_timer {
private:
    boost::asio::io_service io;
    boost::asio::deadline_timer timer;

public:
    door_timer();

    void start();

    void reset();

    bool is_running();
};