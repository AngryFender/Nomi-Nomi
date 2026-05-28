#ifndef CONNECTOR_H
#define CONNECTOR_H
#include "iconnection.h"
#include "itimer.h"

class InitConnector
{
public:
    InitConnector(const std::shared_ptr<IConnection>& connection,
              const tcp::endpoint& endpoint,
              std::unique_ptr<ITimer> timer,
              const std::shared_ptr<ITimer>& repeat_timer
    ): ssl_connection_(connection),
       timer_(std::move(timer)),
       repeat_timer_(repeat_timer)
    {
        ssl_connection_->set_send_callback([this](const boost::system::error_code& err)
        {
            this->sent(err);
        });

        ssl_connection_->set_receive_callback([this](std::unique_ptr<std::vector<char>> payload)
        {
            this->received(std::move(payload));
        });

        auto repeater(repeat_timer_);
        timer_->set_callback([repeater](const boost::system::error_code& err)
        {
            repeater->cancel();
        });

        auto ssl_connect = ssl_connection_;
        repeat_timer_->set_callback([ssl_connect](const boost::system::error_code& err)
        {
            ssl_connect->async_send(std::vector{'P','I','N','G'});
        });

        ssl_connection_->async_connect(endpoint, [](const boost::system::error_code& code)
        {
        });

        ssl_connection_->async_send(std::vector{'P','I','N','G'});
        timer_->start();
        repeat_timer->start();
    }

private:
    std::shared_ptr<IConnection> ssl_connection_;
    std::shared_ptr<ITimer> timer_;
    std::shared_ptr<ITimer> repeat_timer_;
    void received(std::unique_ptr<std::vector<char>> payload);
    void sent(const boost::system::error_code& err);

};



#endif //CONNECTOR_H
