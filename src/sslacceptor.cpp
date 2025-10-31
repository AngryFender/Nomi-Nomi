#include "sslacceptor.h"
#include <fmtlog.h>
#include "sslconnection.h"

void SSLAcceptor::setHandler(std::function<void(std::shared_ptr<IConnection>)> handler)
{
    _accept_handler = handler;
}

void SSLAcceptor::open()
{
    std::shared_ptr<IConnection> ssl_socket = std::make_shared<SSLConnection>(_io_context, _ssl_context);
    logi("Listening ssl port{} server type {}" , _port, static_cast<int>(_type));
    _acceptor.async_accept( ssl_socket->get_socket(), [ ssl_socket, this](const boost::system::error_code& error)
    {
        if (error || !ssl_socket->on_accept())
        {
            loge("error while accepting incoming ssl connnection:  {}", error.message());
            return;
        }
        _accept_handler( ssl_socket);
        this->open();
    });}
