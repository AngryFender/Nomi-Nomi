#ifndef MOCKCONNECTION_H
#define MOCKCONNECTION_H
#include "../../src/iconnection.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

class MockConnection: public IConnection
{
public:
    MOCK_METHOD(tcp::socket&,get_socket,(),(override));
    MOCK_METHOD(bool,on_accept,(),(override));
    MOCK_METHOD(void, set_receive_callback, (std::function<void(std::unique_ptr<std::vector<char>>)> callback), (override));
    MOCK_METHOD(void, set_send_callback, (std::function <void(const boost::system::error_code&)> callback), (override));
    MOCK_METHOD(void,open,(),(override));
    MOCK_METHOD(void, async_send, (const std::vector<char>& packet), (override));
    MOCK_METHOD(void, async_connect, (const tcp::endpoint& endpoint, std::function<void(const boost::system::error_code&)> callback), (override));
    MOCK_METHOD(void, close, (), (override));
};

#endif //MOCKCONNECTION_H
