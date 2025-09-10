#include "manager.h"

void Manager::AddConnection(const tcp::endpoint& endpoint, std::shared_ptr<IConnection> socket)
{
    std::string address_port = endpoint.address().to_string() +":" + std::to_string(endpoint.port());
    socket->async_connect(endpoint, [address_port, socket, this](const boost::system::error_code& code)
    {
        if(code)
        {
            //todo log error message
            return;
        }
        _out_connections[address_port] = socket;

        socket->set_receive_callback([this, address_port](std::vector<char>& buffer)
        {
            //todo delegate the callback to logic object
        });

        socket->set_send_callback([this, address_port](const boost::system::error_code& code)
        {
            if(code)
            {
                //todo log error message
            }
        });
        socket->open();
    });
}

bool Manager::RemoveConnection(const std::string& address_port)
{
    if(_out_connections.contains(address_port))
    {
        _out_connections.erase(address_port);
        return true;
    }
    return false;
}

void Manager::AcceptConnection(const std::shared_ptr<IConnection>& socket)
{
    const std::string address_port = socket->get_socket().remote_endpoint().address().to_string() + ":" +
        std::to_string(socket->get_socket().remote_endpoint().port());

    if(!_out_connections.contains(address_port))
    {
        _out_connections[address_port] = socket;

        socket->set_receive_callback([this, address_port](std::vector<char>& buffer)
        {
            //todo delegate the callback to logic object
        });

        socket->set_send_callback([this, address_port](const boost::system::error_code& code)
        {
            //todo log error message
        });
    }
}

void Manager::ClearAllConnections()
{
    _out_connections.clear();
}

void Manager::ReplyMessage(const std::string& address_port, const std::vector<char>& buffer)
{

}
