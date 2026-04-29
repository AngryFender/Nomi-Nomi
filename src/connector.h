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
    { }

    bool start() override
    {
        //TODO
        return false;
    }

private:
    std::shared_ptr<IConnection> ssl_connection_;
    std::unique_ptr<ITimer> timer_;
    std::unique_ptr<ITimer> repeat_timer_;


};



#endif //CONNECTOR_H
