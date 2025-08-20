#include "manager.h"

void Manager::AddConnection(const tcp::endpoint& endpoint, std::shared_ptr<IConnection> socket)
{

}

bool Manager::RemoveConnection(const std::string& address)
{

}

void Manager::AcceptConnection(const std::shared_ptr<IConnection>& socket)
{
    const std::string address_port = socket->get_socket().remote_endpoint().address().to_string() + ":" +
        std::to_string(socket->get_socket().remote_endpoint().port());

    if(!_out_connections.contains(address_port))
    {
        _out_connections[address_port] = socket;
    }
}

void Manager::ClearAllConnections()
{

}

void Manager::ReplyMessage(const std::string& address_port, const std::vector<char>& buffer)
{

}
