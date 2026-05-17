
#include "connector.h"

void Connector::received(std::unique_ptr<std::vector<char>> data)
{
    if(data->data() == "PONG")
    {
        this->repeat_timer_->cancel();
        this->timer_->cancel();
    }
}

void Connector::sent(const boost::system::error_code& err)
{

}
