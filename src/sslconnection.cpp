#include "sslconnection.h"
#include <fmtlog.h>

tcp::socket& SSLConnection::get_socket()
{
    return _ssl_socket.next_layer();
}

bool SSLConnection::on_accept()
{
    try
    {
        _ssl_socket.handshake(boost::asio::ssl::stream_base::server);
    }
    catch (const std::exception& ex)
    {
        loge("Error when ssl {}", ex.what());
        return false;
    }
    return true;
}

void SSLConnection::set_receive_callback(std::function<void(std::unique_ptr<std::vector<char>>)> callback)
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

    async_read(_ssl_socket, boost::asio::buffer(size_buffer.get(), sizeof(uint32_t)),
        [size_buffer,self](const boost::system::error_code& err, std::size_t size)
    {
        if(err)
        {
            //todo handle error
            loge("Error when receiving packets{}", err.message());
            return;
        }

        // get the message length and create message buffer of the same length
        const uint32_t message_length = ntohl(*size_buffer);
        auto message_buffer = std::make_unique<std::vector<char>>(message_length);

        // read until the message buffer is filled
        async_read(self->_ssl_socket, boost::asio::buffer(*message_buffer),
        [self, &message_buffer](const boost::system::error_code& code, std::size_t size)
        {
            self->_receive_callback(std::move(message_buffer));
            self->open();
        });
    });

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
    auto self = shared_from_this();
    _ssl_socket.lowest_layer().async_connect( endpoint, [self, callback](const boost::system::error_code& error)
    {
        if(error)
        {
            callback(error);
            return;
        }
        self->_ssl_socket.async_handshake(boost::asio::ssl::stream_base::client, callback);
    });
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

void SSLConnection::start_async_send()
{
    auto self = shared_from_this();
    async_write(_ssl_socket, boost::asio::buffer(_outbounds.front()), [self](const boost::system::error_code& error, size_t size)
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
