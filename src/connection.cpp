#include "connection.h"

tcp::socket& Connection::get_socket()
{
    return _socket;
}

void Connection::set_receive_callback(std::function<void(std::vector<char>&)> callback)
{
    _receive_callback = callback;
}

void Connection::set_send_callback(std::function<void(const boost::system::error_code&)> callback)
{
    _send_callback = callback;
}

void Connection::open()
{

}

void Connection::async_send(const std::vector<char>& packet)
{
    _outbounds.push(packet);
    if(!_write_in_progress)
    {
        _write_in_progress = true;
        start_async_send();
    }
}

void Connection::async_connect(const tcp::endpoint& endpoint,
                               std::function<void(const boost::system::error_code&)> callback)
{
    _socket.async_connect(endpoint, callback);
}

void Connection::close()
{
    _socket.close();
}

void Connection::start_async_send()
{
    auto self = shared_from_this();
    const auto buffer = boost::asio::buffer(_outbounds.front());
    async_write(_socket, buffer, [self](const boost::system::error_code& error, size_t size)
    {
        self->_outbounds.pop();
        if(self->_outbounds.empty() || error)
        {
            self->_write_in_progress = false;
            self->_send_callback(error);
            return;
        }
        self->start_async_send();
    });
}
