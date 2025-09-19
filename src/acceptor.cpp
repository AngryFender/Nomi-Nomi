#include "acceptor.h"
#include "connection.h"

void Acceptor::setHandler(std::function<void(std::shared_ptr<IConnection>)> handler)
{
    _accept_handler = handler;
}

void Acceptor::open()
{
    std::shared_ptr<IConnection> socket_base = std::make_shared<Connection>(_io_context);

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
