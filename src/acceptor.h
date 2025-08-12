#ifndef ACCEPTOR_H
#define ACCEPTOR_H
#include "iacceptor.h"


class Acceptor: public IAcceptor {
public:
    Acceptor(boost::asio::io_context& io_context, const boost::asio::ip::port_type port): _io_context(io_context),
        _acceptor(_io_context, tcp::endpoint(tcp::v4(), port))
    {}
    ~Acceptor() override = default;
    void setHandler(std::function<void(std::shared_ptr<IConnection>)> handler) override;
    void open() override;
private:
    boost::asio::io_context& _io_context;
    tcp::acceptor _acceptor;
    std::function<void(std::shared_ptr<IConnection>)> _accept_handler;
};



#endif //ACCEPTOR_H
