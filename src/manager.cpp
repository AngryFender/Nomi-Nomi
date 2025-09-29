#include "manager.h"

void Manager::AddClient(const tcp::endpoint& endpoint, std::shared_ptr<IConnection> socket)
{
    std::string address_port = endpoint.address().to_string() +":" + std::to_string(endpoint.port());
    socket->async_connect(endpoint, [address_port, socket, this](const boost::system::error_code& code)
    {
        if(code)
        {
            //todo log error message
            return;
        }
        _client_connections[address_port] = socket;
        socket->set_receive_callback([this, address_port](daemon_type, std::shared_ptr<std::vector<char>> buffer)
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

bool Manager::RemoveClient(const std::string& address_port)
{
    if(_client_connections.contains(address_port))
    {
        _client_connections.erase(address_port);
        return true;
    }
    return false;
}

void Manager::AcceptClient(const std::shared_ptr<IConnection>& socket)
{
    const std::string address_port = socket->get_socket().remote_endpoint().address().to_string() + ":" +
        std::to_string(socket->get_socket().remote_endpoint().port());

    if(!_client_connections.contains(address_port))
    {
        _client_connections[address_port] = socket;

        socket->set_receive_callback([this, address_port](daemon_type, std::shared_ptr<std::vector<char>>buffer)
        {
            //todo delegate the callback to logic object
        });

        socket->set_send_callback([this, address_port](const boost::system::error_code& code)
        {
            //todo log error message
        });
    }
}

void Manager::ClearAllClients()
{
    _client_connections.clear();
}

void Manager::AddNode(const tcp::endpoint& endpoint, std::shared_ptr<IConnection> socket)
{
    //todo
    std::string address_port = endpoint.address().to_string() + ":" + std::to_string(endpoint.port());
    socket->async_connect(endpoint, [address_port, socket, this](const boost::system::error_code& code)
    {
        if(code)
        {
            //todo log error message
            return;
        }
        _node_connections[address_port] = socket;

        socket->set_receive_callback([this, address_port](daemon_type, std::shared_ptr<std::vector<char>> buffer)
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

bool Manager::RemoveNode(const std::string& address_port)
{
    //todo
    if(_node_connections.contains(address_port))
    {
        _node_connections.erase(address_port);
        return true;
    }
    return false;
}

void Manager::AcceptNode(const std::shared_ptr<IConnection>& socket)
{
    //todo
    const std::string address_port = socket->get_socket().remote_endpoint().address().to_string() + ":" +
        std::to_string(socket->get_socket().remote_endpoint().port());

    if(!_node_connections.contains(address_port))
    {
        _node_connections[address_port] = socket;

        socket->set_receive_callback([this, address_port](daemon_type, std::shared_ptr<std::vector<char>> buffer)
        {
            //todo delegate the callback to logic object
        });

        socket->set_send_callback([this, address_port](const boost::system::error_code& code)
        {
            //todo log error message
        });
    }
}

void Manager::ClearAllNodes()
{
    //todo
    _node_connections.clear();
}

void Manager::ReplyMessage(const std::string& address_port, const std::vector<char>& buffer)
{

}
