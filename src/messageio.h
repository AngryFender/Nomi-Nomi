#ifndef MESSAGEIO_H
#define MESSAGEIO_H
#include <cstdint>
#include "imessageio.h"


class Messageio : public IMessageio{
public:
    ~Messageio() override = default;
    bool read_message(const uint8_t message_type, std::vector<char>& packet_data) override;
};



#endif //MESSAGEIO_H
