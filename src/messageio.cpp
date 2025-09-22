#include "messageio.h"
#include "messagetype.h"

bool Messageio::read_message(const uint8_t message_type, std::vector<char>& packet_data)
{
    switch (static_cast<enum message_type>(message_type))
    {
    case message_type::node_ack:
    case message_type::node_duplicate:
    case message_type::client_set:
    case message_type::client_read:
    default: ;
    }
    return true;
}
