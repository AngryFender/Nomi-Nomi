#ifndef ICONNECTION_H
#define ICONNECTION_H
#include <functional>
#include <boost/asio.hpp>
#include "daemoninfo.h"

using boost::asio::ip::tcp;

class IConnection
{
public:
    virtual ~IConnection() = default;
    virtual IConnection& operator=(const IConnection& other) = delete ;
    virtual IConnection& operator=(IConnection&& other) noexcept = 0 ;
    virtual tcp::socket& get_socket() = 0;
    virtual bool on_accept() = 0;
    virtual void set_receive_callback(std::function <void(std::shared_ptr<std::vector<char>>)> callback) = 0;
    virtual void set_send_callback(std::function <void(const boost::system::error_code&)> callback) = 0;
    virtual void open() = 0;
    virtual void async_send(const std::vector<char>& packet) = 0;
    virtual void async_connect(const tcp::endpoint& endpoint, std::function<void(const boost::system::error_code&)> callback ) = 0;
    virtual void close() = 0;
};

#endif //ICONNECTION_H
