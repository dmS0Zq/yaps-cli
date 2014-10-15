#include "FileIO.h"

namespace FileIO
{

std::string LongData::toString()
{
    return std::string(m_data, m_length);
}

LongData::LongData(std::string str)
{
    m_length = str.length();
    m_data = const_cast<char*>(str.c_str());
}
LongData::LongData(u32 length, byte *data)
{
    m_length = length;
    m_data = data;
}
LongData::~LongData()
{

}

std::ofstream& operator<<(std::ofstream &os, LongData data)
{
    os << data.m_length;
    os << data.m_data;
    return os;
}

std::ofstream &operator<<(std::ofstream &os, uint8_t data)
{
    byte bytes[1];
    bytes[0] = data;
    os.write(bytes, 1);
    return os;
}

std::ofstream &operator<<(std::ofstream &os, uint16_t data)
{
    byte bytes[2];
    bytes[0] = (data) & 0xff;
    bytes[1] = (data >> 8) & 0xff;
    os.write(bytes, 2);
    return os;
}

std::ofstream &operator<<(std::ofstream &os, uint32_t data)
{
    byte bytes[4];
    bytes[0] = (data) & 0xffff;
    bytes[1] = (data >> 8) & 0xffff;
    bytes[2] = (data >> 16) & 0xffff;
    bytes[3] = (data >> 24) & 0xffff;
    os.write(bytes, 4);
    return os;
}

std::ofstream &operator<<(std::ofstream &os, uint64_t data)
{
    byte bytes[8];
    bytes[0] = (data) & 0xffffff;
    bytes[1] = (data >> 8) & 0xffffff;
    bytes[2] = (data >> 16) & 0xffffff;
    bytes[3] = (data >> 24) & 0xffffff;
    bytes[4] = (data >> 32) & 0xffffff;
    bytes[5] = (data >> 40) & 0xffffff;
    bytes[6] = (data >> 48) & 0xffffff;
    bytes[7] = (data >> 56) & 0xffffff;
    os.write(bytes, 8);
    return os;
}

std::ifstream &operator>>(std::ifstream &is, LongData &data)
{
    u32 length;
    is >> length;
    byte* bytes = new byte[length];
    is.read(bytes, length);
    LongData ld(length, bytes);
    data = ld;
    return is;
}

std::ifstream &operator>>(std::ifstream &is, uint8_t &data)
{
    byte bytes[1];
    is.read(bytes, 1);
    data = ((uint8_t)bytes[0] << 0 & 0xff);
    return is;
}

std::ifstream &operator>>(std::ifstream &is, uint16_t &data)
{
    byte bytes[2];
    is.read(bytes, 2);
    data = ((uint16_t)bytes[1] << 8 & 0xff00) |
           ((uint16_t)bytes[0] << 0 & 0x00ff);
    return is;
}

std::ifstream &operator>>(std::ifstream &is, uint32_t &data)
{
    byte bytes[4];
    is.read(bytes, 4);
    data = ((uint32_t)bytes[3] << 24 & 0xff000000) |
           ((uint32_t)bytes[2] << 16 & 0x00ff0000) |
           ((uint32_t)bytes[1] <<  8 & 0x0000ff00) |
           ((uint32_t)bytes[0] <<  0 & 0x000000ff);
    return is;
}

std::ifstream &operator>>(std::ifstream &is, uint64_t &data)
{
    byte bytes[8];
    is.read(bytes, 8);
    data = ((uint64_t)bytes[7] << 56 & 0xff00000000000000) |
           ((uint64_t)bytes[6] << 48 & 0x00ff000000000000) |
           ((uint64_t)bytes[5] << 40 & 0x0000ff0000000000) |
           ((uint64_t)bytes[4] << 32 & 0x000000ff00000000) |
           ((uint64_t)bytes[3] << 24 & 0x00000000ff000000) |
           ((uint64_t)bytes[2] << 16 & 0x0000000000ff0000) |
           ((uint64_t)bytes[1] <<  8 & 0x000000000000ff00) |
           ((uint64_t)bytes[0] <<  0 & 0x00000000000000ff);
    return is;
}


};
