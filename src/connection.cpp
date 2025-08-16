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
    auto self = shared_from_this();
    _socket.async_read_some(boost::asio::buffer(_temp_data), [self](const boost::system::error_code& err, std::size_t size)
    {
        if(err)
        {
            //handle error
            return;
        }

        self->_temp_data.resize(size);

        //implement framing layer
        //add to persistent buffer
        for(const char& item: self->_temp_data)
        {
            self->_internal_buffer.push_back(item);
        }

        //try to parse the message
        if(self->_reader->read_packets(self->_internal_buffer, self->_packet_data))
        {
            self->_receive_callback(self->_packet_data);
            self->_packet_data.clear();
        }

        self->open();
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
