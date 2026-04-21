#ifndef CONNECTOR_H
#define CONNECTOR_H
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/basic_endpoint.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/stream.hpp>

#include "iconnector.h"
#include "sslconnection.h"

class Connector:public IConnector {
public:
    Connector(boost::asio::io_context& io_context,
              const std::string& address,
              const boost::asio::ip::port_type port):
        _ssl_context(boost::asio::ssl::context::tls_client),
        _ssl_socket(io_context, _ssl_context),
        _end_point(boost::asio::ip::address::from_string(address), port)
    {
        _ssl_connection = std::make_shared<SSLConnection>(io_context, _ssl_context);
    }
    bool start() override
    {
        //TODO
        return false;
    }
private:
    boost::asio::ssl::context _ssl_context;
    boost::asio::ssl::stream<boost::asio::ip::tcp::socket> _ssl_socket ;
    boost::asio::ip::basic_endpoint<boost::asio::ip::tcp> _end_point;
    std::shared_ptr<IConnection> _ssl_connection;

};



#endif //CONNECTOR_H
