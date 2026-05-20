#ifndef MOCKTIMER_H
#define MOCKTIMER_H
#include "../../src/itimer.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

class MockTimer : public ITimer {
public:
    MOCK_METHOD(void, start, (), (override));
    MOCK_METHOD(void, cancel, (), (override));
    MOCK_METHOD(void, set_callback, (std::function<void(const boost::system::error_code&)> callback), (override));
};

#endif //MOCKTIMER_H
