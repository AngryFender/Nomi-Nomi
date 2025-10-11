#include <iostream>
#include <boost/asio/io_context.hpp>
#include "fmtlog.h"
#include "fmtlog-inl.h"
#include "../src/connection.h"
// #include "message.capnp.h" not being generated
#include <capnp/message.h>
#include <capnp/serialize.h>

#define SERVER_PORT 3498

int main()
{
    fmtlog::setLogFile("/dev/stdout", false);
    fmtlog::startPollingThread();
    logi("Starting Nomi-Nomi Client...");
    boost::asio::io_context io_context;

    //create socket connection to server and set callbacks
    const std::string address = "127.0.0.1";
    boost::asio::ip::basic_endpoint<tcp> end_point(boost::asio::ip::address::from_string(address), SERVER_PORT);
    auto connection = std::make_shared<Connection>(io_context);
    connection->async_connect(end_point, [address](const boost::system::error_code& err)
    {
        logi("Connected to server @{}", address);
    });

    connection->set_receive_callback([address](std::shared_ptr<std::vector<char>> raw)
    {
        logi("Recieved message from {}", address);
        //TODO deserialise Cap'n Proto message
    });

    connection->set_send_callback([address](const boost::system::error_code& err)
    {
        if(err)
        {
            loge("Error when sending message to {}, error = {}", address, err.message());
        }
    });

    //TODO serialise message into cap'n proto message
    capnp::MallocMessageBuilder message;

    //TODO send the message via socket

    io_context.run();
    fmtlog::stopPollingThread();
    return 0;
}
