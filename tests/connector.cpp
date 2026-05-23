#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <functional>
#include "../src/itimer.h"
#include "../src/connector.h"
#include "mocks/mockconnection.h"
#include "mocks/mocktimer.h"

TEST(ConnectorTest, initialisation_test)
{
    const auto mock_connection = std::make_shared<MockConnection>();
    const auto mock_repeat_timer = std::make_shared<MockTimer>();
    auto mock_timer = std::make_unique<MockTimer>();
    auto mock_timer_view = mock_timer.get();

    std::function<void(const boost::system::error_code& code)> captured_timer_callback;

    EXPECT_CALL(*mock_connection,set_send_callback(testing::_));
    EXPECT_CALL(*mock_connection,set_receive_callback(testing::_));
    EXPECT_CALL(*mock_repeat_timer,set_callback(testing::_));
    EXPECT_CALL(*mock_timer_view,set_callback(testing::_));

    EXPECT_CALL(*mock_connection, async_send(std::vector{'P','I','N','G'}));
    EXPECT_CALL(*mock_repeat_timer, start());
    EXPECT_CALL(*mock_timer_view, start());

    Connector connector(mock_connection, std::move(mock_timer), mock_repeat_timer);
}


