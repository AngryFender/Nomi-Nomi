#include "manager.h"

void Manager::AddConnection(const tcp::endpoint& endpoint, std::shared_ptr<IConnection> socket)
{

}

bool Manager::RemoveConnection(const std::string& address)
{

}

void Manager::AcceptConnection(const std::shared_ptr<IConnection>& socket)
{

}

void Manager::ClearAllConnections()
{

}

void Manager::ReplyMessage(const std::string& address_port, const std::vector<char>& buffer)
{

}
