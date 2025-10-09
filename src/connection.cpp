#include "connection.h"
#include "utility/capnpreader.h"
#include "utility/packetreader.h"

tcp::socket& Connection::get_socket()
{
    return _socket;
}

void Connection::set_receive_callback(std::function<void(std::shared_ptr<std::vector<char>>)> callback)
{
    _receive_callback = callback;
}

void Connection::set_send_callback(std::function<void(const boost::system::error_code&)> callback)
{
    _send_callback = callback;
}

void Connection::open()
{
    auto self = shared_from_this();
    auto len_buffer = std::make_shared<uint32_t>(0);

    async_read(_socket, boost::asio::buffer(len_buffer.get(), sizeof(uint32_t)),
        [len_buffer,self](const boost::system::error_code& err, std::size_t size)
    {
        if(err)
        {
            //todo handle error
            return;
        }

        // get the message length and create message buffer of the same length
        const uint32_t message_length = ntohl(*len_buffer);
        auto message_buffer = std::make_shared<std::vector<char>>(message_length);

        // read until the message buffer is filled
        async_read(self->_socket, boost::asio::buffer(*message_buffer),
        [self, message_buffer](const boost::system::error_code& code, std::size_t size)
        {
            self->_receive_callback(message_buffer);
            self->open();
        });
    });
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
