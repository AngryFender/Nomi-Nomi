#include "repeattimer.h"

void RepeatTimer::start()
{
    timer_.expires_after(period_);
    if(callback_)
    {
        timer_.async_wait([this](const boost::system::error_code& error)
        {
            internal_callback(error);
        });
    }
}

void RepeatTimer::cancel()
{
    timer_.cancel();
}

void RepeatTimer::set_callback(std::function<void(const boost::system::error_code&)> callback)
{
    callback_ = std::move(callback);
}

void RepeatTimer::internal_callback(const boost::system::error_code& error)
{
    if(error == boost::asio::error::operation_aborted)
    {
        return;
    }

    if(callback_)
    {
        callback_(error);
    }
    start();
}
