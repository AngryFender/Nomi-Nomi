#ifndef TIMER_H
#define TIMER_H
#include "../src/itimer.h"

class Timer: public ITimer {
public:
    void start() override;
    void stop() override;
    void cancel() override;
    void set_callback(std::function<void(const boost::system::error_code&)> callback) override;
};



#endif //TIMER_H
