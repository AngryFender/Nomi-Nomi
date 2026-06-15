#ifndef CONNECTION_H
#define CONNECTION_H
#include <queue>
#include "iconnection.h"
#include <boost/circular_buffer.hpp>

class Connection final : public IConnection, public std::enable_shared_from_this<Connection> {
public:
    explicit Connection(tcp::socket&& socket): _socket(std::move(socket)), _write_in_progress(false), _internal_buffer(_buffer_size)
    {
    }

    explicit Connection(boost::asio::io_context& context): _socket(context),_write_in_progress(false), _internal_buffer(_buffer_size)
    {
    }

    Connection(const Connection& other) = delete;
    Connection(Connection&& other) = delete;
    IConnection& operator=(const IConnection& other) override = delete ;
    IConnection& operator=(IConnection&& other) override = delete;
    ~Connection() override = default;

    bool on_accept() override;
    std::string get_address() override;
    void set_receive_callback(std::function<void(std::string_view)> callback) override;
    void set_send_callback(std::function <void(const boost::system::error_code&)> callback) override;
    void open() override;
    void async_send(const std::vector<char>& packet) override;
    void async_connect(const tcp::endpoint& endpoint, std::function<void(const boost::system::error_code&)> callback ) override;
    void close() override;

private:
    const uint8_t _buffer_size{64};
    tcp::socket _socket;
    bool _write_in_progress;
    std::array<char, 4096> _internal_array;
    size_t _read_index = 0;
    size_t _write_index = 0;
    size_t _message_size = 0;
    boost::circular_buffer<char> _internal_buffer;
    std::vector<char> _packet_data;
    std::vector<char> _temp_data;
    std::queue<std::vector<char>> _outbounds;
    std::function <void(const boost::system::error_code&)> _send_callback;
    std::function <void(std::string_view)> _receive_callback;

    void start_async_send();
};



#endif //CONNECTION_H
