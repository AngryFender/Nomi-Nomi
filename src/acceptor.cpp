#include "acceptor.h"
#include "connection.h"

void Acceptor::setHandler(std::function<void(std::shared_ptr<Iconnection>)> handler)
{
    _accept_handler = handler;
}

void Acceptor::open()
{
    std::shared_ptr<Iconnection> socket_base = std::make_shared<Connection>(_io_context);

    //async_accept
    // _acceptor.async_accept()
}
