#include "acceptor.h"
#include <fmtlog.h>
#include "connection.h"

void Acceptor::setHandler(std::function<void(std::shared_ptr<IConnection>)> handler)
{
    _accept_handler = handler;
}

void Acceptor::open()
{
    logi("Listening port{} server type {}" , _port, static_cast<int>(_type));

    _acceptor.async_accept([this](const boost::system::error_code& error, tcp::socket new_socket)
    {
        if (error)
        {
            loge("error while accepting incoming connection:  {}", error.message());
        }

        _accept_handler(std::make_shared<Connection>(std::move(new_socket)));

        this->open();
    });
}
