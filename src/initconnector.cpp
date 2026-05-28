
#include "initconnector.h"

void InitConnector::received(std::unique_ptr<std::vector<char>> data)
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
