#include <iostream>
#include <openssl/provider.h>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ssl.hpp>
#include "fmtlog.h"
#include "fmtlog-inl.h"
#include "message.capnp.h"
#include <capnp/message.h>
#include <capnp/serialize.h>
#include <CLI/App.hpp>
#include <toml++/toml.hpp>

#include "../src/utility/config.h"
#include "../src/sslconnection.h"
#include "../src/utility/capnpreader.h"
#include "../src/utility/packetreader.h"


std::optional<ClientConfig> read_config(std::string_view config_path)
{
    toml::table table;
    try
    {
        table = toml::parse_file(config_path);
    }
    catch (const toml::parse_error& e)
    {
        loge("Config file parsing failed : {}", e.what());
        return std::nullopt;
    }

    ClientConfig config;
    config.self_signed_crt_path = table["ssl"]["self_signed_crt_path"].value_or("");
    config.server_address = table["server"]["address"].value_or("");
    config.server_port = table["server"]["port"].value_or(0);
    config.standby_address = table["standby"]["address"].value<std::string>();
    config.standby_port = table["standby"]["port"].value<int>();
    return config;
}

int main(int argc, char* argv[])
{
    fmtlog::setLogFile("/dev/stdout", false);
    fmtlog::startPollingThread();
    logi("Starting Nomi-Nomi Client...");

    CLI::App app{"Nomi client"};
    CLIArgs args;
    app.add_option("-c,--config",
        args.config_path, "Path to the configuration file")->required(true);
    CLI11_PARSE(app, argc, argv);
    auto config = read_config("");
    if (!config)
    {
        return -1;
    }

    //create socket connection to server and set callbacks
    const std::string& address = config->server_address;
    boost::asio::ip::basic_endpoint<tcp> end_point(boost::asio::ip::address::from_string(address),
                                                   config->server_port);

    OSSL_PROVIDER_load(NULL, "default");
    OSSL_PROVIDER_load(NULL, "legacy");
    boost::asio::ssl::context ssl_context(boost::asio::ssl::context::tls_client);
    ssl_context.set_verify_mode(boost::asio::ssl::verify_peer);
    ssl_context.load_verify_file(config->self_signed_crt_path);

    boost::asio::io_context io_context;
    boost::asio::ssl::stream<tcp::socket> ssl_socket(io_context, ssl_context);

    auto ssl_connection = std::make_shared<SSLConnection>(io_context, ssl_context);
    ssl_connection->async_connect(end_point, [address](const boost::system::error_code& error)
    {
        logi("Securely Connected to server @{}", address);
    });

    ssl_connection->set_receive_callback([address](std::shared_ptr<std::vector<char>> buffer)
    {
        logi("Recieved message from {}", address);
        auto msg = utility::deserialise_message(*buffer);
        logi("message id:{}, resource id:{}, type: {}, userid: {} ", msg.getId(), msg.getResourceid(), msg.getType(),
             msg.getUserid());
    });

    ssl_connection->set_send_callback([address](const boost::system::error_code& err)
    {
        if (err)
        {
            loge("Error when sending message to {}, error = {}", address, err.message());
        }
    });

    auto work = make_work_guard(io_context);

    std::thread t1([&]()
    {
        while (true)
        {
            std::this_thread::sleep_for(std::chrono::seconds(5));

            //TODO serialise message into cap'n proto message
            capnp::MallocMessageBuilder message;
            Message::Builder builder = message.initRoot<Message>();
            builder.setId(69);
            builder.setType(1);
            builder.setUserid(100);
            builder.setResourceid(36);

            //TODO send the message via socket
            ssl_connection->async_send(utility::serialise_message(message));
        }
    });

    io_context.run();
    t1.join();

    fmtlog::stopPollingThread();
    return 0;
}
