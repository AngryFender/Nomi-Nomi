#ifndef CAPNPREADER_H
#define CAPNPREADER_H
#include <message.capnp.h>
#include <capnp/serialize.h>
#include <kj/common.h>
#include "packetreader.h"
namespace utility
{
    inline Message::Reader deserialise_message(std::vector<char>& buffer)
    {
        const auto* raw = reinterpret_cast<const capnp::word*>(buffer.data());
        size_t wordCount = buffer.size() / sizeof(capnp::word);
        kj::ArrayPtr<const capnp::word> view(raw, wordCount);
        capnp::FlatArrayMessageReader reader(view);
        return reader.getRoot<Message>();
    }

    // convert capnp type message into std::vector<char>
    inline std::vector<char> serialise_message(capnp::MallocMessageBuilder& message)
    {
        std::vector<char> payload;

        kj::Array<capnp::word> words = capnp::messageToFlatArray(message);
        kj::ArrayPtr<const capnp::word> view = words.asPtr();
        kj::ArrayPtr<const unsigned char> bytes = kj::arrayPtr(reinterpret_cast<const kj::byte*>(view.begin()),
                                                               view.size() * sizeof(capnp::word));
        uint32_t capn_message_length = htonl(bytes.size());
        payload.reserve(sizeof(capn_message_length) + bytes.size());
        append_bytes(payload, capn_message_length);
        payload.insert(payload.end(), bytes.begin(), bytes.end());

        return payload;
    }
}

#endif //CAPNPREADER_H
