
#include "initconnector.h"

void InitConnector::received(std::string_view data)
{
    if(data == "PONG")
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
    connection_->async_connect(endpoint_,[this](const boost::system::error_code& error)
    {
        if(error)
        {
            connection_->close();
        }
        else
        {
            send_ini_message();
        }
    });
}

void InitConnector::send_ini_message()
{
    connection_->async_send(std::vector{'P','I','N','G'});
}


