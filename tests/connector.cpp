#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <functional>
#include <boost/system/error_code.hpp> // Required for your specific signature
#include "../src/itimer.h"

class MockTimer : public ITimer {
public:
    MOCK_METHOD(void, start, (), (override));
    MOCK_METHOD(void, cancel, (), (override));
    MOCK_METHOD(void, set_callback, (std::function<void(const boost::system::error_code&)> callback), (override));
};

TEST(ConnectorTest, timer_timeout) {
    // Arrange: Create the mock and the class under test

}


