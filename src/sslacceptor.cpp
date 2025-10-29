#include "sslacceptor.h"
#include <fmtlog.h>
#include "sslconnection.h"

void SSLAcceptor::setHandler(std::function<void(std::shared_ptr<IConnection>)> handler)
{
    _accept_handler = handler;
}

void SSLAcceptor::open()
{
    std::shared_ptr<IConnection> socket_base = std::make_shared<SSLConnection>(_io_context, _ssl_context);
    logi("Listening ssl port{} server type {}" , _port, static_cast<int>(_type));
    //async_accept
    _acceptor.async_accept(socket_base->get_socket(), [socket_base, this](const boost::system::error_code& error)
    {
        if (error)
        {
            loge("error while accepting incoming ssl connnection:  {}", error.message());
        }
        _accept_handler(socket_base);

        this->open();
    });}
