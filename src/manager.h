#ifndef MANAGER_H
#define MANAGER_H
#include "imanager.h"

class Manager: public IManager {

public:
    explicit Manager(){}
    ~Manager() override = default;
    void AddConnection(const tcp::endpoint& endpoint, std::shared_ptr<IConnection> socket) override;
    bool RemoveConnection(const std::string& address) override;
    void AcceptConnection(const std::shared_ptr<IConnection>& socket) override;
    void ClearAllConnections() override;
    void ReplyMessage(const std::string& address_port, const std::vector<char>& buffer) override;

};



#endif //MANAGER_H
