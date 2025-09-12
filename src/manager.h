#ifndef MANAGER_H
#define MANAGER_H
#include "iacceptor.h"
#include "imanager.h"
#include "ireader.h"

class Manager: public IManager {

public:
    explicit Manager(std::unique_ptr<IAcceptor> client_acceptor, std::unique_ptr<IAcceptor> node_acceptor): _client_acceptor(std::move(client_acceptor)), _node_acceptor(std::move(node_acceptor))
    {
        _client_acceptor->setHandler([this](const std::shared_ptr<IConnection>& socket)
        {
            this->AcceptClient(socket);
        });

    }
    ~Manager() override = default;
    void AddClient(const tcp::endpoint& endpoint, std::shared_ptr<IConnection> socket) override;
    bool RemoveClient(const std::string& address_port) override;
    void AcceptClient(const std::shared_ptr<IConnection>& socket) override;
    void ClearAllClients() override;
    void AddNode(const tcp::endpoint& endpoint, std::shared_ptr<IConnection> socket) override;
    bool RemoveNode(const std::string& address_port) override;
    void AcceptNode(const std::shared_ptr<IConnection>& socket) override;
    void ClearAllNodes() override;

    void ReplyMessage(const std::string& address_port, const std::vector<char>& buffer) override;
private:
    std::unique_ptr<IAcceptor> _client_acceptor;
    std::unique_ptr<IAcceptor> _node_acceptor;
    std::unordered_map<std::string, std::shared_ptr<IConnection>> _out_connections;

};



#endif //MANAGER_H
