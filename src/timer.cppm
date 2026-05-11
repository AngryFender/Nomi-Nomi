module;
#include <boost/asio/io_context.hpp>
#include <boost/asio/steady_timer.hpp>

export module timer;
import itimer;
export class Timer: public ITimer {
public:
    Timer(boost::asio::io_context& io_context, boost::asio::chrono::seconds period): timer_(io_context), period_(period)
    {
    }

    void start() override
    {
        timer_.expires_after(period_);
        if (callback_)
        {
            timer_.async_wait(callback_);
        }
    }

    void cancel() override
    {
        timer_.cancel();
    }

    void set_callback(std::function<void(const boost::system::error_code&)> callback) override
    {
        callback_ = std::move(callback);
    }

private:
    boost::asio::steady_timer timer_;
    boost::asio::chrono::seconds period_;
    std::function<void(const boost::system::error_code&)> callback_;
};

