#include <iostream>
#include <openssl/provider.h>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ssl.hpp>
#include "fmtlog.h"
#include "fmtlog-inl.h"
#include "message.capnp.h"
#include <capnp/message.h>
#include <capnp/serialize.h>

#include "../src/sslconnection.h"
#include "../src/utility/capnpreader.h"
#include "../src/utility/packetreader.h"

constexpr int SERVER_PORT = 3491;

int main()
{
    fmtlog::setLogFile("/dev/stdout", false);
    fmtlog::startPollingThread();
    logi("Starting Nomi-Nomi Client...");
    boost::asio::io_context io_context;

    //create socket connection to server and set callbacks
    const std::string address = "127.0.0.1";
    boost::asio::ip::basic_endpoint<tcp> end_point(boost::asio::ip::address::from_string(address), SERVER_PORT);

    OSSL_PROVIDER_load(NULL, "default");
    OSSL_PROVIDER_load(NULL, "legacy");
    boost::asio::ssl::context ssl_context(boost::asio::ssl::context::tls_client);
    ssl_context.set_verify_mode(boost::asio::ssl::verify_peer);

    const char* ssl_path = std::getenv("SSL_PATH");
    if(!ssl_path)
    {
        logi("SSL_PATH environment variable not set");
        return -1;
    }
    const std::string ca_path = std::string(ssl_path) + "server.crt";
    ssl_context.load_verify_file(ca_path);

    boost::asio::ssl::stream<tcp::socket> ssl_socket(io_context, ssl_context);

    auto ssl_connection = std::make_shared<SSLConnection>(io_context, ssl_context);
    ssl_connection->async_connect(end_point, [address](const boost::system::error_code& error)
    {
        logi("Securely Connected to server @{}", address);
    });

    ssl_connection->set_receive_callback([address](std::shared_ptr<std::vector<char>> buffer)
    {
        logi("Recieved message from {}", address);
        auto msg  = utility::deserialise_message(*buffer);
        logi("message id:{}, resource id:{}, type: {}, userid: {} ", msg.getId(), msg.getResourceid(), msg.getType(),
             msg.getUserid());
    });

    ssl_connection->set_send_callback([address](const boost::system::error_code& err)
    {
        if(err)
        {
            loge("Error when sending message to {}, error = {}", address, err.message());
        }
    });

    auto work = make_work_guard(io_context);

    std::thread t1([&]()
    {
        while(true)
        {
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
    }) ;

    io_context.run();
    t1.join();

    fmtlog::stopPollingThread();
    return 0;
}
