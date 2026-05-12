#ifndef CONNECTOR_H
#define CONNECTOR_H
#include "iconnection.h"
#include "iconnector.h"
#include "itimer.h"

class Connector : public IConnector
{
public:
    Connector(const std::shared_ptr<IConnection>& connection,
              std::unique_ptr<ITimer> timer,
              std::unique_ptr<ITimer> repeat_timer
    ): ssl_connection_(connection),
       timer_(std::move(timer)),
       repeat_timer_(std::move(repeat_timer))
    {
        ssl_connection_->set_send_callback([this](const boost::system::error_code& err)
        {
            this->sent(err);
        });

        ssl_connection_->set_receive_callback([this](std::unique_ptr<std::vector<char>> payload)
        {
            this->received(std::move(payload));
        });

    }

    bool start() override
    {
        //TODO
        return false;
    }

private:
    std::shared_ptr<IConnection> ssl_connection_;
    std::shared_ptr<ITimer> timer_;
    std::shared_ptr<ITimer> repeat_timer_;
    void received(std::unique_ptr<std::vector<char>> payload);
    void sent(const boost::system::error_code& err);


};



#endif //CONNECTOR_H
