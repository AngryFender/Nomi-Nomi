#include <boost/asio/io_context.hpp>
#include "../src/daemoninfo.h"
#include "../src/acceptor.h"
#include "../src/manager.h"
#include <fmtlog.h>
#include "fmtlog-inl.h"

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
    auto client_acceptor = std::make_unique<Acceptor>(daemon_type::client, io_context,SERVER_LISTENING_PORT);
    auto node_acceptor = std::make_unique<Acceptor>(daemon_type::node, io_context, NODE_PORT);

    Manager manager(std::move(client_acceptor),std::move(node_acceptor),CLIENT_THREAD_MAX, NODE_THREAD_MAX);
    io_context.run();
    fmtlog::stopPollingThread();
    return 0;
}
