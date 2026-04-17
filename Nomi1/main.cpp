#include <boost/asio/io_context.hpp>
#include <openssl/provider.h>
#include <CLI/CLI.hpp>
#include <toml++/toml.hpp>
#include "../src/daemoninfo.h"
#include "../src/acceptor.h"
#include "../src/manager.h"
#include <fmtlog.h>
#include <iostream>

#include "fmtlog-inl.h"
#include "../src/sslacceptor.h"
#include "../src/utility/config.h"

constexpr int SERVER1_LISTENING_PORT = 4500;
constexpr int SERVER2_LISTENING_PORT = 4501;
constexpr int CLIENT_THREAD_MAX = 2;
constexpr int NODE1_PORT = 3500;
constexpr int NODE2_PORT = 3501;
constexpr int NODE_THREAD_MAX = 2;

std::optional<Config> read_config(std::string_view config_path)
{
    toml::table config_file;
    try
    {
        config_file = toml::parse_file(config_path);
    }
    catch (toml::parse_error& e)
    {
        logi("Config file parsing error: {}", e.what());
        return std::nullopt;
    }

    Config config;
    config.server_cert_path = config_file["server"]["cert_path"].value_or("");
    config.server_key_path = config_file["server"]["key_path"].value_or("");
    config.type = config_file["server"]["type"].value_or(1);
    config.server_port = config_file["server"]["port"].value_or(0);
    config.server_threads = config_file["server"]["server_threads"].value_or(0);
    config.standby_address = config_file["standby"]["address"].value<std::string>();
    config.standby_port = config_file["standby"]["port"].value<int>();
    config.standby_threads = config_file["standby"]["thread_max"].value<int>();
    config.is_standby_configured = config.standby_cert_path && config.standby_address && config.standby_port;
    return config;
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

    OSSL_PROVIDER_load(nullptr, "default");

    auto ctx_server = create_ssl_context(config->server_cert_path,
                                                config->server_key_path);

    boost::asio::io_context context_server;
    auto client_acceptor = std::make_unique<SSLAcceptor>(daemon_type::client,
                                                         context_server,
                                                         ctx_server,
                                                         SERVER1_LISTENING_PORT);
    std::unique_ptr<SSLAcceptor> standby_acceptor;
    boost::asio::io_context context_standby;
    if (config->type && config->is_standby_configured)
    {
        auto ctx_standby = create_ssl_context(config->server_cert_path,
                                              config->server_key_path);

        standby_acceptor = std::make_unique<SSLAcceptor>(daemon_type::client,
                                                              context_server,
                                                              ctx_standby,
                                                              config->standby_port.value_or(0));


    }

    Manager manager(std::move(client_acceptor),std::move(standby_acceptor),CLIENT_THREAD_MAX, NODE_THREAD_MAX);

    std::thread thread_client([&]()
    {
        context_server.run();
    });
    std::thread thread_node([&]()
    {
        context_standby.run();
    });

    thread_client.join();
    thread_node.join();
    fmtlog::stopPollingThread();
    return 0;
}
