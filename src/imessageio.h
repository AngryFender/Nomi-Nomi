#ifndef IREADER_H
#define IREADER_H
#include <vector>

class IMessageio{
public:
    virtual ~IMessageio() = default;
    virtual bool read_message(const uint8_t message_type, std::vector<char>& packet_data) = 0;
};

#endif //IREADER_H
