#ifndef CONNECTOR_H
#define CONNECTOR_H
#include <utility>

#include "iconnection.h"
#include "itimer.h"

class InitConnector
{
public:
    InitConnector(const std::shared_ptr<IConnection>& connection,
                  tcp::endpoint endpoint,
              std::unique_ptr<ITimer> timer,
              const std::shared_ptr<ITimer>& repeat_timer
    ): connection_(connection),
       endpoint_(std::move(endpoint)),
       timer_(std::move(timer)),
       repeat_timer_(repeat_timer)
    {
        connection_->set_send_callback([this](const boost::system::error_code& err)
        {
            this->sent(err);
        });

        connection_->set_receive_callback([this](std::string_view payload)
        {
            this->received(payload);
        });

        auto repeater(repeat_timer_);
        timer_->set_callback([repeater, connect = connection_](const boost::system::error_code& err)
        {
            repeater->cancel();
            connect->close();
        });

        repeat_timer_->set_callback([&](const boost::system::error_code& err)
        {
             if(err)
             {
                 init_connect();
             }
             else
             {
                 send_ini_message();
             }
        });

        init_connect();
    }

private:
    std::shared_ptr<IConnection> connection_;
    std::shared_ptr<ITimer> timer_;
    std::shared_ptr<ITimer> repeat_timer_;
    tcp::endpoint endpoint_;
    void received(std::string_view payload);
    void sent(const boost::system::error_code& err);
    void init_connect();
    void send_ini_message();
};



#endif //CONNECTOR_H
