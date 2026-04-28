#ifndef REPEATTIMER_H
#define REPEATTIMER_H
#include <boost/asio/io_context.hpp>
#include <boost/asio/steady_timer.hpp>

#include "itimer.h"

class RepeatTimer: public ITimer {
public:
    RepeatTimer(boost::asio::io_context& io_context, boost::asio::chrono::seconds period): timer_(io_context), period_(period)
    {

    }
    void start() override;
    void cancel() override;
    void set_callback(std::function<void(const boost::system::error_code&)> callback) override;
private:
    boost::asio::steady_timer timer_;
    boost::asio::chrono::seconds period_;
    std::function<void(const boost::system::error_code&)> callback_;
    void internal_callback(const boost::system::error_code&);
};



#endif //REPEATTIMER_H
