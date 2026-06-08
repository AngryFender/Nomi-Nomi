
#include "initconnector.h"

void InitConnector::received(std::string_view payload) const
{
    if(payload == "PONG")
    {
        this->repeat_timer_->cancel();
        this->timer_->cancel();
        logi("PONG received");
    }
}

void InitConnector::sent(const boost::system::error_code& err)
{

}

void InitConnector::init_connect() const
{
    connection_->async_connect(endpoint_,[this](const boost::system::error_code& error)
    {
        if(error)
        {
            if (error == boost::asio::error::already_connected)
            {
                send_ini_message();
                return;
            }
            loge("Init connect err :{} {}", error.value(), error.message());
            return;
        }
        send_ini_message();
    });
}

void InitConnector::send_ini_message() const
{
    connection_->async_send(std::vector{'P','I','N','G'});
}


