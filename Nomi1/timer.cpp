#include "timer.h"

void Timer::start()
{
    timer_.expires_after(period_);
    if(callback_)
    {
        timer_.async_wait(callback_);
    }
}

void Timer::cancel()
{
    timer_.cancel();
}

void Timer::set_callback(std::function<void(const boost::system::error_code&)> callback)
{
    callback_ = std::move(callback);
}
