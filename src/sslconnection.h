#ifndef SSLCONNECTION_H
#define SSLCONNECTION_H
#include <queue>
#include <boost/circular_buffer.hpp>
#include <boost/asio/ssl/stream.hpp>

#include "iconnection.h"

class SSLConnection: public IConnection, public std::enable_shared_from_this<SSLConnection> {
public:
    explicit SSLConnection(boost::asio::io_context& context, boost::asio::ssl::context ssl_context):
        _ssl_socket(context, ssl_context), _write_in_progress(false)
    {

    }
    ~SSLConnection() override = default;
    tcp::socket& get_socket() override;
    bool on_accept() override;
    void set_receive_callback(std::function<void(std::shared_ptr<std::vector<char>>)> callback) override;
    void set_send_callback(std::function<void(const boost::system::error_code&)> callback) override;
    void open() override;
    void async_send(const std::vector<char>& packet) override;
    void async_connect(const tcp::endpoint& endpoint,
                       std::function<void(const boost::system::error_code&)> callback) override;
    void close() override;
private:
    boost::asio::ssl::stream<tcp::socket> _ssl_socket;
    bool _write_in_progress;
    boost::circular_buffer<char> _internal_buffer;
    std::vector<char> _packet_data;
    std::vector<char> _temp_data;
    std::queue<std::vector<char>> _outbounds;
    std::function <void(const boost::system::error_code&)> _send_callback;
    std::function <void(std::shared_ptr<std::vector<char>>)> _receive_callback;

    void start_async_send();};



#endif //SSLCONNECTION_H
