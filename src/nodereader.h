#ifndef NODEREADER_H
#define NODEREADER_H
#include "ireader.h"
#include "nodereader.h"

class NodeReader : public IReader{
public:
    NodeReader()=default;
    ~NodeReader() override = default;
    bool read_packets(boost::circular_buffer<char>& buffer, std::vector<char>& packet_data) override;
};



#endif //NODEREADER_H
