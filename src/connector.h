#ifndef CONNECTOR_H
#define CONNECTOR_H
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/basic_endpoint.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/stream.hpp>

#include "iconnector.h"
#include "sslconnection.h"
#include "../src/iconnection.h"

class Connector:public IConnector {
public:
    Connector(boost::asio::io_context& io_context,
        boost::asio::ssl::context& ssl_context):
        _ssl_connection(std::make_shared<SSLConnection>( io_context, ssl_context))
    {
        _ssl_connection = std::make_shared<SSLConnection>(io_context, ssl_context);
    }
    bool start() override
    {
        //TODO
        return false;
    }
private:
    std::shared_ptr<IConnection> _ssl_connection;

};



#endif //CONNECTOR_H
