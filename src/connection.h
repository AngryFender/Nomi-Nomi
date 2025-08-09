#ifndef CONNECTION_H
#define CONNECTION_H
#include "iconnection.h"


class Connection : public Iconnection {
public:
    explicit Connection()
    {

    }
    ~Connection() override = default;
    void set_receive_callback(std::function <void(std::vector<char>&)> callback) override;
    void set_send_callback(std::function <void(const boost::system::error_code&)> callback) override;
    void open() override;
    void async_send(const std::vector<char>& packet) override;
    void async_connect(const tcp::endpoint& endpoint, std::function<void(const boost::system::error_code&)> callback ) override;
    void close() override;

private:

};



#endif //CONNECTION_H
