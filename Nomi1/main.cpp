#include <iostream>
#include <boost/asio/io_context.hpp>
#include "../src/daemoninfo.h"
#include "../src/acceptor.h"
#include "../src/manager.h"

#define CLIENT_PORT 3498
#define CLIENT_THREAD_MAX 2
#define NODE_PORT 3499
#define NODE_THREAD_MAX 2

int main()
{
    std::cout << "Nomi-Nomi server init...." << std::endl;

    boost::asio::io_context io_context;
    auto client_acceptor = std::make_unique<Acceptor>(daemon_type::client, io_context,CLIENT_PORT);
    auto node_acceptor = std::make_unique<Acceptor>(daemon_type::client, io_context, NODE_PORT);

    Manager manager(std::move(client_acceptor),std::move(node_acceptor),CLIENT_THREAD_MAX, NODE_THREAD_MAX);
    io_context.run();
    return 0;
}
