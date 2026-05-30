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
    ): connection_(connection),
       endpoint_(endpoint),
       timer_(std::move(timer)),
       repeat_timer_(repeat_timer)
    {
        connection_->set_send_callback([this](const boost::system::error_code& err)
        {
            this->sent(err);
        });

        connection_->set_receive_callback([this](std::unique_ptr<std::vector<char>> payload)
        {
            this->received(std::move(payload));
        });

        auto repeater(repeat_timer_);
        timer_->set_callback([repeater, connect = connection_](const boost::system::error_code& err)
        {
            repeater->cancel();
            connect->close();
        });

        repeat_timer_->set_callback([&,connect = connection_](const boost::system::error_code& err)
        {
             if(err)
             {
                 init_connect();
             }
             else
             {
                 connection_->async_send(std::vector{'P', 'I', 'N', 'G'});
             }
        });

        init_connect();
    }

private:
    std::shared_ptr<IConnection> connection_;
    std::shared_ptr<ITimer> timer_;
    std::shared_ptr<ITimer> repeat_timer_;
    tcp::endpoint endpoint_;
    void received(std::unique_ptr<std::vector<char>> payload);
    void sent(const boost::system::error_code& err);
    void init_connect();
};



#endif //CONNECTOR_H
