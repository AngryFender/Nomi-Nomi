#include <boost/asio/io_context.hpp>
#include <openssl/provider.h>
#include "../src/daemoninfo.h"
#include "../src/acceptor.h"
#include "../src/manager.h"
#include <fmtlog.h>
#include "fmtlog-inl.h"
#include "../src/sslacceptor.h"

constexpr int SERVER1_LISTENING_PORT = 3491;
constexpr int SERVER2_LISTENING_PORT = 3492;
constexpr int CLIENT_THREAD_MAX = 2;
constexpr int NODE1_PORT = 3498;
constexpr int NODE2_PORT = 3499;
constexpr int NODE_THREAD_MAX = 2;

int main()
{
    fmtlog::setLogFile("/dev/stdout", false);
    fmtlog::startPollingThread();
    logi("Starting Nomi-Nomi server...");

    boost::asio::io_context context_client;
    boost::asio::io_context context_node;

    OSSL_PROVIDER_load(nullptr, "default");
    OSSL_PROVIDER_load(nullptr, "legacy");

    boost::asio::ssl::context ssl_context(boost::asio::ssl::context::tls_server);

    ssl_context.set_options(
        boost::asio::ssl::context::default_workarounds |
        boost::asio::ssl::context::no_sslv2 |
        boost::asio::ssl::context::no_sslv3 |
        boost::asio::ssl::context::no_tlsv1 |
        boost::asio::ssl::context::no_tlsv1_1 |
        boost::asio::ssl::context::single_dh_use);

    const char* ssl_path = std::getenv("SSL_PATH");
    if(!ssl_path)
    {
        logi("SSL_PATH environment variable not set");
        return -1;
    }

    const std::string cert_path = std::string(ssl_path) + "server.crt";
    const std::string key_path = std::string(ssl_path) + "server.key";

    ssl_context.use_certificate_chain_file(cert_path);
    ssl_context.use_private_key_file(key_path, boost::asio::ssl::context::pem);

    auto client_acceptor = std::make_unique<SSLAcceptor>(daemon_type::client, context_client, ssl_context, SERVER2_LISTENING_PORT);
    auto node_acceptor = std::make_unique<Acceptor>(daemon_type::node, context_node, NODE2_PORT);

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
