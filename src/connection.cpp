#include <fmtlog.h>
#include "connection.h"
#include "utility/capnpreader.h"
#include "utility/packetreader.h"

bool Connection::on_accept()
{
    return true;
}

std::string Connection::get_address()
{
    const std::string address_port = _socket.remote_endpoint().address().to_string() + ":" +
        std::to_string(_socket.remote_endpoint().port());
    return address_port;
}

void Connection::set_receive_callback(std::function<void(std::string_view)> callback)
{
    _receive_callback = callback;
}

void Connection::set_send_callback(std::function<void(const boost::system::error_code&)> callback)
{
    _send_callback = callback;
}

void Connection::open()
{
    async_read(
        _socket,
        boost::asio::buffer(_internal_array.data() + _write_index, _internal_array.size() - _write_index),
        [self = shared_from_this()](const boost::system::error_code& err, const std::size_t size)
    {
        if(err)
        {
            //TODO error handling
            switch (err.value())
            {
            case boost::asio::error::not_connected:
                logd("Not Connected");
                break;
            case boost::asio::error::operation_aborted:
                logd("Connection closed");
                return;
            default:
                loge("Error when receiving packets: {}", err.message());
                return;
            }
        }
        //accumulate all the received bytes sizes
        self->_write_index += size;

        //check if all the bytes arrived
        if(self->_write_index >= self->_internal_array[self->_read_index])
        {
            self->_receive_callback(std::string_view(self->_internal_array.data() + self->_read_index, size));
        }
        //TODO reset the buffer

        self->open();

        // get the message length and create message buffer of the same length
        // const int8_t message_length = self->_internal_buffer.front();
        // self->_internal_buffer.pop_front();
        //
        // // read until the message buffer is filled
        // async_read(self->_socket, boost::asio::buffer(&self->_internal_buffer.front(), sizeof(char)),
        // [self, message_length](const boost::system::error_code& code, std::size_t size)
        // {
        //     self->_receive_callback(std::string_view(&self->_internal_buffer.front(), message_length));
        //     self->_internal_buffer.clear();
        //     self->open();
        // });
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
    async_write(_socket, boost::asio::buffer(_outbounds.front()), [self = shared_from_this()](const boost::system::error_code& error, size_t size)
    {
        if(error)
        {
            loge("Error while sending bytes: {}", error.message());
        }
        self->_outbounds.pop();
        if(self->_outbounds.empty() || error)
        {
            self->_write_in_progress = false;
            if (error) self->_send_callback(error);
            return;
        }
        self->start_async_send();
    });
    logi("sent package size {}", _outbounds.front().size());
}
