#include "connection.h"

void Connection::set_receive_callback(std::function<void(std::vector<char>&)> callback)
{
}

void Connection::set_send_callback(std::function<void(const boost::system::error_code&)> callback)
{
}

void Connection::open()
{
}

void Connection::async_send(const std::vector<char>& packet)
{
}

void Connection::async_connect(const tcp::endpoint& endpoint,
                               std::function<void(const boost::system::error_code&)> callback)
{
}

void Connection::close()
{
}
