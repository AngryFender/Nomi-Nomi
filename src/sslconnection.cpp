#include "sslconnection.h"

tcp::socket& SSLConnection::get_socket()
{
}

void SSLConnection::set_receive_callback(std::function<void(std::shared_ptr<std::vector<char>>)> callback)
{
    _receive_callback = callback;
}

void SSLConnection::set_send_callback(std::function<void(const boost::system::error_code&)> callback)
{
    _send_callback = callback;
}

void SSLConnection::open()
{
    auto self = shared_from_this();
    auto size_buffer = std::make_shared<uint32_t>(0);
}

void SSLConnection::async_send(const std::vector<char>& packet)
{
    _outbounds.push(packet);
    if(!_write_in_progress)
    {
        _write_in_progress = true;
        start_async_send();
    }
}

void SSLConnection::async_connect(const tcp::endpoint& endpoint,
    std::function<void(const boost::system::error_code&)> callback)
{

}

void SSLConnection::close()
{
    auto self = shared_from_this();
    _ssl_socket.async_shutdown([self](const boost::system::error_code& code)
    {
        if(code)
        {
            //TODO error reporting
        }
        boost::system::error_code ignored;
        self->_ssl_socket.lowest_layer().close(ignored);
    });
}
