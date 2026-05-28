#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <functional>
#include "../src/initconnector.h"
#include "mocks/mockconnection.h"
#include "mocks/mocktimer.h"

TEST(ConnectorTest, initialisation_test)
{
    const auto mock_connection = std::make_shared<MockConnection>();
    const auto mock_repeat_timer = std::make_shared<MockTimer>();
    auto mock_timer = std::make_unique<MockTimer>();
    const auto mock_timer_view = mock_timer.get();
    const tcp::endpoint endpoint(boost::asio::ip::address::from_string("0.0.0.0"), 3000);

    EXPECT_CALL(*mock_connection, set_send_callback(testing::_));
    EXPECT_CALL(*mock_connection, set_receive_callback(testing::_));
    EXPECT_CALL(*mock_connection, async_connect(endpoint, testing::_));
    EXPECT_CALL(*mock_repeat_timer, set_callback(testing::_));
    EXPECT_CALL(*mock_timer_view, set_callback(testing::_));

    EXPECT_CALL(*mock_connection, async_send(std::vector{'P','I','N','G'}));
    EXPECT_CALL(*mock_repeat_timer, start());
    EXPECT_CALL(*mock_timer_view, start());

    InitConnector connector(mock_connection, endpoint, std::move(mock_timer), mock_repeat_timer);
}

TEST(ConnectorTest, pong_test)
{
    const auto mock_connection = std::make_shared<MockConnection>();
    const auto mock_repeat_timer = std::make_shared<MockTimer>();
    auto mock_timer = std::make_unique<MockTimer>();
    const auto mock_timer_view = mock_timer.get();
    const tcp::endpoint endpoint(boost::asio::ip::address::from_string("0.0.0.0"), 3000);

    EXPECT_CALL(*mock_connection, set_send_callback(testing::_));
    EXPECT_CALL(*mock_connection, async_send(std::vector{'P','I','N','G'}));
    EXPECT_CALL(*mock_repeat_timer, set_callback(testing::_));
    EXPECT_CALL(*mock_timer_view, set_callback(testing::_));
    EXPECT_CALL(*mock_repeat_timer, start());
    EXPECT_CALL(*mock_timer_view, start());

    std::function<void(std::unique_ptr<std::vector<char>> data)> captured_set_receive_callback;
    EXPECT_CALL(*mock_connection, set_receive_callback(testing::_))
        .WillOnce(testing::SaveArg<0>(&captured_set_receive_callback));
    EXPECT_CALL(*mock_connection, async_connect(endpoint, testing::_));

    InitConnector connector(mock_connection, endpoint, std::move(mock_timer), mock_repeat_timer);

    EXPECT_CALL(*mock_repeat_timer, cancel()).Times(1);
    EXPECT_CALL(*mock_timer_view, cancel()).Times(1);

    std::vector fake_data{'P', 'O', 'N', 'G'};
    auto fake_packets = std::make_unique<std::vector<char>>(std::move(fake_data));
    captured_set_receive_callback(std::move(fake_packets));

}
TEST(ConnectorTest, no_pong_test)
{
    const auto mock_connection = std::make_shared<MockConnection>();
    const auto mock_repeat_timer = std::make_shared<MockTimer>();
    auto mock_timer = std::make_unique<MockTimer>();
    const auto mock_timer_view = mock_timer.get();
    const tcp::endpoint endpoint(boost::asio::ip::address::from_string("0.0.0.0"), 3000);

    EXPECT_CALL(*mock_connection, set_send_callback(testing::_));
    EXPECT_CALL(*mock_connection, set_receive_callback(testing::_));
    EXPECT_CALL(*mock_repeat_timer, set_callback(testing::_));
    EXPECT_CALL(*mock_repeat_timer, start());
    EXPECT_CALL(*mock_timer_view, start());

    std::function<void(const boost::system::error_code& code)> captured_timer_callback;
    EXPECT_CALL(*mock_timer_view, set_callback(testing::_))
        .WillOnce(testing::SaveArg<0>(&captured_timer_callback));
    EXPECT_CALL(*mock_connection, async_connect(endpoint, testing::_));

    EXPECT_CALL(*mock_connection, async_send(std::vector{'P','I','N','G'}));

    InitConnector connector(mock_connection, endpoint, std::move(mock_timer), mock_repeat_timer);

    EXPECT_CALL(*mock_timer_view, cancel()).Times(0);
    EXPECT_CALL(*mock_repeat_timer, cancel()).Times(1);
    // since the 30 seconds timeout is over, the repeated timer is cancelled

    boost::system::error_code code;
    captured_timer_callback(code);


}

