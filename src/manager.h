#ifndef MANAGER_H
#define MANAGER_H
#include "iacceptor.h"
#include "imanager.h"

class Manager: public IManager {

public:
    explicit Manager(std::unique_ptr<IAcceptor> acceptor):_acceptor(std::move(acceptor))
    {
        _acceptor->setHandler([this](const std::shared_ptr<IConnection>& socket)
        {
            this->AcceptConnection(socket);
        });

    }
    ~Manager() override = default;
    void AddConnection(const tcp::endpoint& endpoint, std::shared_ptr<IConnection> socket) override;
    bool RemoveConnection(const std::string& address_port) override;
    void AcceptConnection(const std::shared_ptr<IConnection>& socket) override;
    void ClearAllConnections() override;
    void ReplyMessage(const std::string& address_port, const std::vector<char>& buffer) override;
private:
    std::unique_ptr<IAcceptor> _acceptor;
    std::unordered_map<std::string, std::shared_ptr<IConnection>> _out_connections;

};



#endif //MANAGER_H
