#ifndef REQUESTREADER_H
#define REQUESTREADER_H
#include "ireader.h"

class RequestReader : public IReader{
public:
    RequestReader() = default;
    ~RequestReader() override = default;
    bool read_packets(boost::circular_buffer<char>& buffer, std::vector<char>& packet_data) override;
};

#endif //REQUESTREADER_H
