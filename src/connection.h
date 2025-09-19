#ifndef CONNECTION_H
#define CONNECTION_H
#include <queue>

#include "iconnection.h"
#include <boost/circular_buffer.hpp>
#include "ireader.h"

class Connection : public IConnection, public std::enable_shared_from_this<Connection> {
public:
    explicit Connection(boost::asio::io_context& context): _socket(context),_write_in_progress(false)
    {

    }
    ~Connection() override = default;
    tcp::socket& get_socket() override;
    void set_receive_callback(std::function <void(std::vector<char>&)> callback) override;
    void set_send_callback(std::function <void(const boost::system::error_code&)> callback) override;
    void open() override;
    void async_send(const std::vector<char>& packet) override;
    void async_connect(const tcp::endpoint& endpoint, std::function<void(const boost::system::error_code&)> callback ) override;
    void close() override;

private:
    tcp::socket _socket;
    bool _write_in_progress;
    boost::circular_buffer<char> _internal_buffer;
    std::vector<char> _packet_data;
    std::vector<char> _temp_data;
    std::queue<std::vector<char>> _outbounds;
    std::function <void(const boost::system::error_code&)> _send_callback;
    std::function <void(std::vector<char>&rawData)> _receive_callback;

    void start_async_send();
};



#endif //CONNECTION_H
