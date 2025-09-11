#include <iostream>
#include "message.capnp.h"
#include <capnp/message.h>
#include <capnp/serialize.h>

int main()
{
    std::cout << "Testing Cap'n Proto !" << std::endl;

    capnp::MallocMessageBuilder message;
    Message::Builder builder = message.initRoot<Message>();

    builder.setId(69);
    builder.setUserid(100);
    builder.setResourceid(36);

    // serialise
    auto words = capnp::messageToFlatArray(message);
    kj::ArrayPtr<const capnp::word> view = words.asPtr();

    // deserialise
    capnp::FlatArrayMessageReader reader(view);
    Message::Reader msg = reader.getRoot<Message>();

    std::cout<<"id = "<< msg.getId()
    <<" user id = "<<msg.getUserid()
    <<" resource id = "<<msg.getResourceid();

    return 0;
}
