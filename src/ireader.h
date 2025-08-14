#ifndef IREADER_H
#define IREADER_H
#include <boost/circular_buffer.hpp>
#include <vector>

class IReader{
public:
    virtual ~IReader() = default;
    virtual bool read_packets(boost::circular_buffer<char>& buffer, std::vector<char>& packet_data) = 0;
};

#endif //IREADER_H
