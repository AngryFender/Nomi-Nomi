#ifndef IMANAGER_H
#define IMANAGER_H
#include "iconnection.h"

class IManager
{
public:
    virtual ~IManager() = default;
    virtual void AddConnection(const tcp::endpoint& endpoint, std::shared_ptr<IConnection> socket) = 0;
    virtual bool RemoveConnection(const std::string& address) = 0;
    virtual void AcceptConnection(const std::shared_ptr<IConnection>& socket) = 0;
    virtual void ClearAllConnections() = 0;
    virtual void ReplyMessage(const std::string& address_port, const std::vector<char>& buffer) = 0;
};


#endif //IMANAGER_H
