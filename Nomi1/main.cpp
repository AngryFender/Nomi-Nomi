#include <boost/asio/io_context.hpp>
#include "../src/daemoninfo.h"
#include "../src/acceptor.h"
#include "../src/manager.h"
#include <fmtlog.h>
#include "fmtlog-inl.h"
#include "../src/sslacceptor.h"

constexpr int SERVER_LISTENING_PORT = 3491;
constexpr int CLIENT_THREAD_MAX = 2;
constexpr int NODE_PORT = 3499;
constexpr int NODE_THREAD_MAX = 2;

int main()
{
    fmtlog::setLogFile("/dev/stdout", false);
    fmtlog::startPollingThread();
    logi("Starting Nomi-Nomi server...");

    boost::asio::io_context io_context;
    boost::asio::ssl::context ssl_context(boost::asio::ssl::context::tls_server);
    //TODO set up certificates
    ssl_context.set_default_verify_paths();
    boost::asio::ssl::stream<tcp::socket> ssl_socket(io_context, ssl_context);

    auto client_acceptor = std::make_unique<SSLAcceptor>(daemon_type::client, io_context, ssl_context, SERVER_LISTENING_PORT);
    auto node_acceptor = std::make_unique<SSLAcceptor>(daemon_type::node, io_context, ssl_context, NODE_PORT);

    Manager manager(std::move(client_acceptor),std::move(node_acceptor),CLIENT_THREAD_MAX, NODE_THREAD_MAX);
    io_context.run();
    fmtlog::stopPollingThread();
    return 0;
}
