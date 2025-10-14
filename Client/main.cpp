#include <iostream>
#include <boost/asio/io_context.hpp>
#include "fmtlog.h"
#include "fmtlog-inl.h"
#include "../src/connection.h"
#include "message.capnp.h"
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

    connection->set_receive_callback([address](std::shared_ptr<std::vector<char>> buffer)
    {
        logi("Recieved message from {}", address);
        //TODO deserialise Cap'n Proto message
        const auto* raw = reinterpret_cast<const capnp::word*>(buffer->data());
        size_t wordCount = buffer->size() / sizeof(capnp::word);

        kj::ArrayPtr<const capnp::word> view(raw, wordCount);
        capnp::FlatArrayMessageReader reader(view);
        auto msg = reader.getRoot<Message>();
        logi("message id:{}, resource id:{}, type: {}, userid: {} ", msg.getId(), msg.getResourceid(), msg.getType(),
             msg.getUserid());
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
    Message::Builder builder = message.initRoot<Message>();
    builder.setId(69);
    builder.setType(1);
    builder.setUserid(100);
    builder.setResourceid(36);

    kj::Array<capnp::word> words = capnp::messageToFlatArray(message);
    kj::ArrayPtr<const capnp::word> view = words.asPtr();
    kj::ArrayPtr<const unsigned char> bytes = kj::arrayPtr(reinterpret_cast<const kj::byte*>(view.begin()), view.size() * sizeof(capnp::word));
    std::vector<char> payload(bytes.begin(), bytes.end());

    std::thread t1([&]()
    {
        std::this_thread::sleep_for(std::chrono::seconds(5));

        //TODO send the message via socket
        connection->async_send(payload);
    }) ;

    io_context.run();
    t1.join();

    fmtlog::stopPollingThread();
    return 0;
}
