#include "acceptor.h"
#include <fmtlog.h>
#include "connection.h"

void Acceptor::setHandler(std::function<void(std::shared_ptr<IConnection>)> handler)
{
    _accept_handler = handler;
}

void Acceptor::open()
{
    std::shared_ptr<IConnection> socket_base = std::make_shared<Connection>(_type,_io_context);
    logi("Listening port{} server type {}" , _port, static_cast<int>(_type));
    //async_accept
    _acceptor.async_accept(socket_base->get_socket(), [socket_base, this](const boost::system::error_code& error)
    {
        if (error)
        {
            //print error
        }
        _accept_handler(socket_base);

        this->open();
    });
}
