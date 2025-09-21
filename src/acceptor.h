#ifndef ACCEPTOR_H
#define ACCEPTOR_H
#include "iacceptor.h"
#include "imessageio.h"

class Acceptor: public IAcceptor {
public:
    Acceptor(boost::asio::io_context& io_context, const boost::asio::ip::port_type port, std::shared_ptr<IMessageio> messageio): _io_context(io_context),
        _acceptor(_io_context, tcp::endpoint(tcp::v4(), port)), _messageio(std::move(messageio))
    {}
    ~Acceptor() override = default;
    void setHandler(std::function<void(std::shared_ptr<IConnection>)> handler) override;
    void open() override;
private:
    boost::asio::io_context& _io_context;
    tcp::acceptor _acceptor;
    std::shared_ptr<IMessageio> _messageio;
    std::function<void(std::shared_ptr<IConnection>)> _accept_handler;
};



#endif //ACCEPTOR_H
