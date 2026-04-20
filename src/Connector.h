#ifndef CONNECTOR_H
#define CONNECTOR_H
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/basic_endpoint.hpp>
#include "iconnector.h"

class Connector:public IConnector {
public:
    Connector( boost::asio::io_context& io_context, const std::string_view address,  const boost::asio::ip::port_type port)
    {

    }
    bool start() override
    {
        //TODO
        return false;
    }
};



#endif //CONNECTOR_H
