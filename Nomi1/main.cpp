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

Config read_config(const std::string& config_path)
{
    Config config;
    auto config_file = toml::parse_file(config_path);
    config.ssl_path = config_file["ssl"]["ssl_path"].value_or("");
    config.primary_cert_path = config_file["primary"]["cert_path"].value_or("");
    config.primary_key_path =  config_file["primary"]["key_path"].value_or("");
    config.primary_server_port = config_file["primary"]["server_port"].value_or(0);
    config.client_thread_max = config_file["primary"]["client_thread_max"].value_or(0);
    config.standby_cert_path =  config_file["standby"]["cert_path"].value_or("");
    config.standby_key_path =  config_file["standby"]["key_path"].value_or("");
    config.standby_server_port = config_file["primary"]["server_port"].value_or(NULL);
    config.standby_thread_max = config_file["primary"]["standby_thread_max"].value_or(NULL);
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

    CLI::App app{"Nomi primary server"};
    CLIArgs args;
    app.add_option("-c,--config", args.config_path, "Path to the configuration file")->required(true);
    CLI11_PARSE(app, argc, argv);

    Config config = read_config(args.config_path);

    boost::asio::io_context context_primary;
    boost::asio::io_context context_standby;
    OSSL_PROVIDER_load(nullptr, "default");
    auto ctx_primary = create_ssl_context(config.primary_cert_path, config.primary_key_path);
    auto ctx_standby = create_ssl_context(config.standby_cert_path, config.standby_key_path);

    auto primary_acceptor = std::make_unique<SSLAcceptor>(daemon_type::client, context_primary, ctx_primary, SERVER1_LISTENING_PORT);
    auto standby_acceptor = config.standby_server_port.value()
                                ? std::make_unique<SSLAcceptor>(daemon_type::client, context_primary, ctx_standby,
                                                                config.standby_server_port.value_or(0))
                                : nullptr;

    Manager manager(std::move(primary_acceptor),std::move(standby_acceptor),CLIENT_THREAD_MAX, NODE_THREAD_MAX);

    std::thread thread_client([&]()
    {
        context_primary.run();
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
