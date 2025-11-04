
#ifndef SSLACCEPTOR_H
#define SSLACCEPTOR_H

#include "iacceptor.h"
#include <boost/asio/ssl/stream.hpp>

class SSLAcceptor : public IAcceptor {
public:
    SSLAcceptor(const daemon_type type, boost::asio::io_context& io_context, boost::asio::ssl::context& ssl_context, const boost::asio::ip::port_type port):
        _type(type), _io_context(io_context),_ssl_context(ssl_context), _port(port),
        _acceptor(_io_context, tcp::endpoint(tcp::v4(), port))
    {}
    ~SSLAcceptor() override = default;
    void setHandler(std::function<void(std::shared_ptr<IConnection>)> handler) override;
    void open() override;
private:
    daemon_type _type;
    boost::asio::io_context& _io_context;
    boost::asio::ssl::context& _ssl_context;
    boost::asio::ip::port_type _port;
    tcp::acceptor _acceptor;
    std::function<void(std::shared_ptr<IConnection>)> _accept_handler;
};



#endif //SSLACCEPTOR_H
