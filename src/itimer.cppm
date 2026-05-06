module;
#include <boost/system/error_code.hpp>

export module itimer;
export class ITimer
{
public:
    virtual ~ITimer() = default;
    virtual void start() = 0;
    virtual void cancel() = 0;
    virtual void set_callback(std::function <void(const boost::system::error_code&)> callback) = 0;
};