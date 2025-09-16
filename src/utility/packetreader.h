#ifndef PACKETREADER_H
#define PACKETREADER_H
#include <vector>
#include <boost/circular_buffer.hpp>

namespace utility
{
    inline bool parse_message(boost::circular_buffer<char>& buffer, std::vector<char>& packet_data)
    {
        if (buffer.size() < 4)
        {
            return false;
        }

        const uint32_t packet_len = (buffer[0] << 24 | buffer[1] << 16 | buffer[2] << 8 | buffer[3]) + 4;

        if (buffer.size() < packet_len)
        {
            return false;
        }

        for (int i = 0; i < packet_len; ++i)
        {
            packet_data.push_back(buffer.front());
            buffer.pop_front();
        }
        return true;
    }

    template <typename T>
    void append_bytes(std::vector<char>& buffer, const T& value)
    {
        static_assert(std::is_trivially_copyable_v<T>, "Type must be trivially copyable");
        const char* ptr = reinterpret_cast<const char*>(&value);
        buffer.insert(buffer.end(), ptr, ptr + sizeof(T));
    }

    template <typename T>
    T bytes_to_uint(const int start, const int end, const std::vector<char>& data)
    {
        T num = 0;
        for (int i = start; i <= end; ++i)
        {
            num = (static_cast<T>(data[i]) << ((i - start) * 8)) | num;
        }
        return num;
    }

    inline uint64_t htonl64(uint64_t value)
    {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
        return (static_cast<uint64_t>(htonl(value & 0xFFFFFFFF)) << 32) | htonl(value >> 32);
#else
        return value;
#endif
    }

    inline uint64_t ntohl64(uint64_t value)
    {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
        return (static_cast<uint64_t>(ntohl(value & 0xFFFFFFFF)) << 32) | ntohl(value >> 32);
#else
        return value;
#endif
    }

}
#endif //PACKETREADER_H
