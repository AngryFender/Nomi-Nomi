#ifndef ICONNECTION_H
#define ICONNECTION_H
#include <functional>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class IConnection
{
public:
    virtual ~IConnection() = default;
    virtual tcp::socket& get_socket() = 0;
    virtual void set_receive_callback(std::function <void(std::vector<char>&)> callback) = 0;
    virtual void set_send_callback(std::function <void(const boost::system::error_code&)> callback) = 0;
    virtual void open() = 0;
    virtual void async_send(const std::vector<char>& packet) = 0;
    virtual void async_connect(const tcp::endpoint& endpoint, std::function<void(const boost::system::error_code&)> callback ) = 0;
    virtual void close() = 0;
};

#endif //ICONNECTION_H
