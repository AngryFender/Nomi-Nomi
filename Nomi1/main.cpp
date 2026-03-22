#include <boost/asio/io_context.hpp>
#include <openssl/provider.h>
#include <CLI/CLI.hpp>
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

int main(int argc, char* argv[])
{
    fmtlog::setLogFile("/dev/stdout", false);
    fmtlog::startPollingThread();
    logi("Starting Nomi-Nomi server...");

    CLI::App app{"Nomi primary server"};
    CLIArgs args;
    app.add_option("-c,--config", args.config_path, "Path to the configuration file")->required(true);
    CLI11_PARSE(app, argc, argv);

    boost::asio::io_context context_client;
    boost::asio::io_context context_node;

    OSSL_PROVIDER_load(nullptr, "default");
    OSSL_PROVIDER_load(nullptr, "legacy");

    const char* ssl_path = std::getenv("SSL_PATH");
    if(!ssl_path)
    {
        logi("SSL_PATH environment variable not set");
        return -1;
    }

    //TODO add config reader
    const std::string cert_path = std::string(ssl_path) + "server.crt";
    const std::string key_path = std::string(ssl_path) + "server.key";
    const std::string cert_node_path = std::string(ssl_path) + "node.crt";
    const std::string key_node_path = std::string(ssl_path) + "node.key";

    //Abstract this to a builder
    boost::asio::ssl::context ssl_server(boost::asio::ssl::context::tls_server);
    ssl_server.set_options(
        boost::asio::ssl::context::default_workarounds |
        boost::asio::ssl::context::no_sslv2 |
        boost::asio::ssl::context::no_sslv3 |
        boost::asio::ssl::context::no_tlsv1 |
        boost::asio::ssl::context::no_tlsv1_1 |
        boost::asio::ssl::context::single_dh_use);
    ssl_server.use_certificate_chain_file(cert_node_path);
    ssl_server.use_private_key_file(key_node_path, boost::asio::ssl::context::pem);

    boost::asio::ssl::context ssl_node(boost::asio::ssl::context::tls_server);
    ssl_node.set_options(
        boost::asio::ssl::context::default_workarounds |
        boost::asio::ssl::context::no_sslv2 |
        boost::asio::ssl::context::no_sslv3 |
        boost::asio::ssl::context::no_tlsv1 |
        boost::asio::ssl::context::no_tlsv1_1 |
        boost::asio::ssl::context::single_dh_use);
    ssl_node.use_certificate_chain_file(cert_path);
    ssl_node.use_private_key_file(key_path, boost::asio::ssl::context::pem);


    auto client_acceptor = std::make_unique<SSLAcceptor>(daemon_type::client, context_client, ssl_server, SERVER1_LISTENING_PORT);
    auto node_acceptor = std::make_unique<SSLAcceptor>(daemon_type::client, context_client, ssl_node, NODE1_PORT);

    Manager manager(std::move(client_acceptor),std::move(node_acceptor),CLIENT_THREAD_MAX, NODE_THREAD_MAX);

    std::thread thread_client([&]()
    {
        context_client.run();
    });
    std::thread thread_node([&]()
    {
        context_node.run();
    });

    thread_client.join();
    thread_node.join();
    fmtlog::stopPollingThread();
    return 0;
}
