#ifndef MANAGER_H
#define MANAGER_H
#include <fmtlog.h>

#include "iacceptor.h"
#include "imanager.h"
#include "concurrentqueue.h"
#include "utility/capnpreader.h"
#include <capnp/message.h>
#include <capnp/serialize.h>

class Manager: public IManager {

public:
    explicit Manager(std::unique_ptr<IAcceptor> client_acceptor,
        std::unique_ptr<IAcceptor> node_acceptor,
        const int client_thread_max,
        const int node_thread_max): _client_acceptor(std::move(client_acceptor)),
                                    _node_acceptor(std::move(node_acceptor)),
                                    _client_threads(client_thread_max),
                                    _node_threads(node_thread_max),
                                    _client_requests(100),
                                    _node_requests(100)
    {
        _client_acceptor->setHandler([this](const std::shared_ptr<IConnection>& socket)
        {
            this->AcceptClient(socket);
        });
        _client_acceptor->open();

        _node_acceptor->setHandler([this](const std::shared_ptr<IConnection>& socket)
        {
           this->AcceptNode(socket) ;
        });
        _node_acceptor->open();

        for (int i = 0; i < client_thread_max; ++i)
        {
            _client_threads.emplace_back([&]()
            {
                while (true)
                {
                    std::shared_ptr<std::vector<char>> request;
                    if (_client_requests.try_dequeue(request))
                    {
                        auto msg = utility::deserialise_message(*request);
                        logi("message id:{}, resource id:{}, type: {}, userid: {} ", msg.getId(), msg.getResourceid(),
                             msg.getType(),
                             msg.getUserid());

                        // //TODO do work after deserialising the message
                    }
                    else
                    {
                        std::this_thread::yield();
                    }
                }
            });
        }
        for (int i = 0; i < node_thread_max; ++i)
        {
            _node_threads.emplace_back([&]()
            {
                while (true)
                {
                    std::shared_ptr<std::vector<char>> request;
                    if (_node_requests.try_dequeue(request))
                    {
                        Message::Reader msg;
                        // utility::deserialise_message(*request, msg);

                        //TODO do work after deserialising the message
                    }
                    else
                    {
                        std::this_thread::yield();
                    }
                }
            });
        }
    }

    ~Manager() override
    {
        for (auto &t : _client_threads)
            t.join();

        for (auto &t : _node_threads)
            t.join();
    };

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
    std::unordered_map<std::string, std::shared_ptr<IConnection>> _client_connections;
    std::unordered_map<std::string, std::shared_ptr<IConnection>> _node_connections;
    std::vector<std::thread> _client_threads;
    std::vector<std::thread> _node_threads;
    moodycamel::ConcurrentQueue<std::unique_ptr<std::vector<char>>> _client_requests;
    moodycamel::ConcurrentQueue<std::unique_ptr<std::vector<char>>> _node_requests;

};



#endif //MANAGER_H
