#ifndef TIMER_H
#define TIMER_H
#include <boost/asio/io_context.hpp>
#include <boost/asio/steady_timer.hpp>

#include "../src/itimer.h"

class Timer: public ITimer {
public:
    Timer(boost::asio::io_context& io_context, boost::asio::chrono::seconds period): timer_(io_context), period_(period)
    {

    }
    void start() override;
    void cancel() override;
    void set_callback(std::function<void(const boost::system::error_code&)> callback) override;
private:
    boost::asio::steady_timer timer_;
    boost::asio::chrono::seconds period_;
    std::function<void(const boost::system::error_code&)> callback_;
};



#endif //TIMER_H
