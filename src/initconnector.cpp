
#include "initconnector.h"

void InitConnector::received(const std::unique_ptr<std::vector<char>>& data)
{
    const std::string_view payload(data->data(), data->size());

    if(payload == "PONG")
    {
        this->repeat_timer_->cancel();
        this->timer_->cancel();
    }
}

void InitConnector::sent(const boost::system::error_code& err)
{

}

void InitConnector::init_connect()
{
    connection_->async_connect(endpoint_,[&](const boost::system::error_code& error)
    {
        if(error)
        {
            connection_->close();
        }
        else
        {
            send_ini_message();
        }
        timer_->start();
        repeat_timer_->start();
    });
}

void InitConnector::send_ini_message()
{
    connection_->async_send(std::vector{'P','I','N','G'});
}


