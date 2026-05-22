#include "sslacceptor.h"
#include <fmtlog.h>
#include "sslconnection.h"

void SSLAcceptor::setHandler(std::function<void(std::shared_ptr<IConnection>)> handler)
{
    _accept_handler = handler;
}

void SSLAcceptor::open()
{
    _acceptor.async_accept([this](const boost::system::error_code& error, tcp::socket&& new_socket)
    {
        if (error)
        {
            loge("error while accepting incoming ssl connnection:  {}", error.message());
        }
        else
        {
            auto ssl_socket = std::make_shared<SSLConnection>(std::move(new_socket),this->_ssl_context);
            if(ssl_socket->on_accept())
            {
                _accept_handler(ssl_socket);
            }
        }
        this->open();
    });}
