#ifndef MOCKTIMER_H
#define MOCKTIMER_H
#include "../../src/itimer.h"

class MockTimer: public ITimer
{
public:
    void start() override
    {
    }

    void cancel() override
    {
    }

    void set_callback(std::function<void(const boost::system::error_code&)> callback) override
    {
    }
};

#endif //MOCKTIMER_H
