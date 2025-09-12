#ifndef IMANAGER_H
#define IMANAGER_H
#include "iconnection.h"

class IManager
{
public:
    virtual ~IManager() = default;
    virtual void AddClient(const tcp::endpoint& endpoint, std::shared_ptr<IConnection> socket) = 0;
    virtual bool RemoveClient(const std::string& address_port) = 0;
    virtual void AcceptClient(const std::shared_ptr<IConnection>& socket) = 0;
    virtual void ClearAllClients() = 0;
    virtual void AddNode(const tcp::endpoint& endpoint, std::shared_ptr<IConnection> socket) = 0;
    virtual bool RemoveNode(const std::string& address_port) = 0;
    virtual void AcceptNode(const std::shared_ptr<IConnection>& socket) = 0;
    virtual void ClearAllNodes() = 0;
    virtual void ReplyMessage(const std::string& address_port, const std::vector<char>& buffer) = 0;
};


#endif //IMANAGER_H
