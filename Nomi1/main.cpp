#include <boost/asio/io_context.hpp>
#include <openssl/provider.h>
#include <CLI/CLI.hpp>
#include <toml++/toml.hpp>
#include "../src/daemoninfo.h"
#include "../src/manager.h"
#include <fmtlog.h>
#include <iostream>

#include "fmtlog-inl.h"
#include "../src/initconnector.h"
#include "../src/repeattimer.h"
#include "../src/sslacceptor.h"
#include "../src/sslconnection.h"
#include "../src/connection.h"
#include "../src/timer.h"
#include "../src/utility/config.h"
#include <glaze/toml.hpp>

constexpr int SERVER1_LISTENING_PORT = 4500;
constexpr int SERVER2_LISTENING_PORT = 4501;
constexpr int CLIENT_THREAD_MAX = 2;
constexpr int NODE1_PORT = 3500;
constexpr int NODE2_PORT = 3501;
constexpr int NODE_THREAD_MAX = 2;

std::optional<Config> read_config(const std::string_view path)
{
    Config config1;
    if(auto error = glz::read_file_toml(config1, path, std::string()))
    {
        logi("Config file parsing error: {}", error.custom_error_message);
        return std::nullopt;
    }
    return config1;
}

boost::asio::ssl::context create_ssl_context(const std::string& cert_path, const std::string& key_path)
{
    boost::asio::ssl::context ctx(boost::asio::ssl::context::tls_server);
    ctx.set_options(
        boost::asio::ssl::context::default_workarounds |
        boost::asio::ssl::context::no_sslv2 |
        boost::asio::ssl::context::no_sslv3 |
        boost::asio::ssl::context::no_tlsv1 |
        boost::asio::ssl::context::no_tlsv1_1 |
        boost::asio::ssl::context::single_dh_use);
    ctx.use_certificate_chain_file(cert_path);
    ctx.use_private_key_file(key_path, boost::asio::ssl::context::pem);
    return ctx;
}

int main(int argc, char* argv[])
{
    fmtlog::setLogFile("/dev/stdout", false);
    fmtlog::startPollingThread();
    logi("Starting Nomi-Nomi server...");

    CLI::App app{"Nomi server"};
    CLIArgs args;
    app.add_option("-c,--config", args.config_path, "Path to the configuration file")->required(true);
    CLI11_PARSE(app, argc, argv);
    const std::optional<Config> config = read_config(args.config_path);
    if(!config)
    {
        return -1;
    }

    // OSSL_PROVIDER_load(nullptr, "default");
    //
    // auto ctx_server = create_ssl_context(config->server.cert_path,
    //                                             config->server.key_path);
    //
    boost::asio::io_context context_server;
    // auto client_acceptor = std::make_unique<SSLAcceptor>(daemon_type::client,
    //                                                      context_server,
    //                                                      ctx_server,
    //                                                      SERVER1_LISTENING_PORT);

    // auto active_node = std::make_unique<SSLConnection>(context_server,ctx_server);
    auto active_node = std::make_unique<Connection>(context_server);
    const auto repeat_timer = RepeatTimer::create(context_server, std::chrono::seconds(config->general.init_repeat_period));
    auto active_connect_timeout = std::make_unique<Timer>(context_server, std::chrono::seconds(config->general.init_timeout));

    const tcp::endpoint endpoint(boost::asio::ip::address::from_string(config->standby->address), config->standby->port);
    InitConnector connector(std::move(active_node), endpoint, std::move(active_connect_timeout), repeat_timer);


    // std::unique_ptr<SSLAcceptor> standby_acceptor;
    // boost::asio::io_context context_standby;
    // if (config->type && config->standby.has_value())
    // {
    //     auto ctx_standby = create_ssl_context(config->standby->cert_path,
    //                                           config->standby->key_path);
    //
    //     standby_acceptor = std::make_unique<SSLAcceptor>(daemon_type::client,
    //                                                           context_server,
    //                                                           ctx_standby,
    //                                                           config->standby->port);
    //
    // }
    //
    // Manager manager(std::move(client_acceptor),std::move(standby_acceptor),CLIENT_THREAD_MAX, NODE_THREAD_MAX);
    //
    // std::thread thread_client([&]()
    // {
    //     context_server.run();
    // });
    // std::thread thread_node([&]()
    // {
    //     context_standby.run();
    // });
    //
    // thread_client.join();
    // thread_node.join();
    context_server.run();
    fmtlog::stopPollingThread();
    return 0;
}
