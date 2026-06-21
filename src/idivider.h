#ifndef IDIVIDER_H
#define IDIVIDER_H
#include <span>

class IDivider
{
public:
    virtual ~IDivider () = default;
    virtual void divide(
        size_t& _write_index,
        size_t& read_index,
        size_t& message_size,
        const size_t received_sized,
        std::span buffer
    ) = 0;
};

#endif //IDIVIDER_H
