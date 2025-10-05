#ifndef CAPNPREADER_H
#define CAPNPREADER_H
#include <message.capnp.h>
#include <capnp/serialize.h>
#include <kj/common.h>
namespace utility
{
    inline bool deserialise_message(std::vector<char>& buffer, Message::Reader& msg)
    {
        //convert to words
        auto words = kj::arrayPtr(reinterpret_cast<const capnp::word*>(buffer.data()),buffer.size()/sizeof(capnp::word));

        //construct a reader
        capnp::FlatArrayMessageReader reader(words);
        msg = reader.getRoot<Message>();

        //todo return the deserialised message
        return true;
    }

    inline bool serialise_message()
    {
        //todo
        return false;
    }
}

#endif //CAPNPREADER_H
