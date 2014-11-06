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
    // not sure if necessary to delete() the data
}

std::ofstream& operator<<(std::ofstream &os, LongData data)
{
    // write out the length (u32) using the u32 overlaod,
    // and then the data.
    os << data.m_length;
    os << data.m_data;
    return os;
}

std::ofstream &operator<<(std::ofstream &os, u8 data)
{
    // could be just the "os.write()" line, but keeping
    // pattern of other u* overloads
    byte bytes[1];
    bytes[0] = data;
    os.write(bytes, 1);
    return os;
}

std::ofstream &operator<<(std::ofstream &os, u16 data)
{
    // store individual bytes,
    // least significant byte first.
    // honestly not sure if a mask needs to be applied.
    // I think not bytes[0] only gets the least significant
    // 8 bits, for example.
    byte bytes[2];
    bytes[0] = (data) ;//& 0xff;
    bytes[1] = (data >> 8);// & 0xff;
    os.write(bytes, 2);
    return os;
}

std::ofstream &operator<<(std::ofstream &os, u32 data)
{
    // store individual bytes,
    // least significant byte first.
    byte bytes[4];
    bytes[0] = (data);// & 0xffff;
    bytes[1] = (data >> 8);// & 0xffff;
    bytes[2] = (data >> 16);// & 0xffff;
    bytes[3] = (data >> 24);// & 0xffff;
    os.write(bytes, 4);
    return os;
}

std::ofstream &operator<<(std::ofstream &os, u64 data)
{
    // store individual bytes,
    // least significant byte first.
    byte bytes[8];
    bytes[0] = (data);// & 0xffffff;
    bytes[1] = (data >> 8);// & 0xffffff;
    bytes[2] = (data >> 16);// & 0xffffff;
    bytes[3] = (data >> 24);// & 0xffffff;
    bytes[4] = (data >> 32);// & 0xffffff;
    bytes[5] = (data >> 40);// & 0xffffff;
    bytes[6] = (data >> 48);// & 0xffffff;
    bytes[7] = (data >> 56);// & 0xffffff;
    os.write(bytes, 8);
    return os;
}

std::ofstream &operator<<(std::ofstream &os, s8 data)
{
    byte bytes[1];
    bytes[0] = data;
    os.write(bytes, 1);
    return os;
}

std::ofstream &operator<<(std::ofstream &os, s16 data)
{
    // store individual bytes,
    // least significant byte first.
    // honestly not sure if a mask needs to be applied.
    // I think not bytes[0] only gets the least significant
    // 8 bits, for example.
    byte bytes[2];
    bytes[0] = (data) ;//& 0xff;
    bytes[1] = (data >> 8);// & 0xff;
    os.write(bytes, 2);
    return os;
}

std::ofstream &operator<<(std::ofstream &os, s32 data)
{
    // store individual bytes,
    // least significant byte first.
    byte bytes[4];
    bytes[0] = (data);// & 0xffff;
    bytes[1] = (data >> 8);// & 0xffff;
    bytes[2] = (data >> 16);// & 0xffff;
    bytes[3] = (data >> 24);// & 0xffff;
    os.write(bytes, 4);
    return os;
}

std::ofstream &operator<<(std::ofstream &os, s64 data)
{
    // store individual bytes,
    // least significant byte first.
    byte bytes[8];
    bytes[0] = (data);// & 0xffffff;
    bytes[1] = (data >> 8);// & 0xffffff;
    bytes[2] = (data >> 16);// & 0xffffff;
    bytes[3] = (data >> 24);// & 0xffffff;
    bytes[4] = (data >> 32);// & 0xffffff;
    bytes[5] = (data >> 40);// & 0xffffff;
    bytes[6] = (data >> 48);// & 0xffffff;
    bytes[7] = (data >> 56);// & 0xffffff;
    os.write(bytes, 8);
    return os;
}

std::ifstream &operator>>(std::ifstream &is, LongData &data)
{
    // first get the length (u32) using u32 overload.
    // then read in that many bytes and construct a new
    // LongData from the information
    u32 length;
    is >> length;
    byte* bytes = new byte[length];
    is.read(bytes, length);
    LongData ld(length, bytes);
    data = ld;
    return is;
}

std::ifstream &operator>>(std::ifstream &is, u8 &data)
{
    // complicated in order to maintain pattern.
    byte bytes[1];
    is.read(bytes, 1);
    data = ((u8)bytes[0] << 0 & 0xff);
    return is;
}

std::ifstream &operator>>(std::ifstream &is, u16 &data)
{
    // read bytes into an array,
    // then restructure them into the data.
    // most significant byte at end.
    // bytes have to be cast to appropriate size,
    // then shifted, then a mask applied to make sure only
    // the 8 original bits are visible.
    byte bytes[2];
    is.read(bytes, 2);
    data = ((u16)bytes[1] << 8 & 0xff00) |
           ((u16)bytes[0] << 0 & 0x00ff);
    return is;
}

std::ifstream &operator>>(std::ifstream &is, u32 &data)
{
    // read bytes into an array,
    // then restructure them into the data.
    // most significant byte at end.
    byte bytes[4];
    is.read(bytes, 4);
    data = ((u32)bytes[3] << 24 & 0xff000000) |
           ((u32)bytes[2] << 16 & 0x00ff0000) |
           ((u32)bytes[1] <<  8 & 0x0000ff00) |
           ((u32)bytes[0] <<  0 & 0x000000ff);
    return is;
}

std::ifstream &operator>>(std::ifstream &is, u64 &data)
{
    // read bytes into an array,
    // then restructure them into the data.
    // most significant byte at end.
    byte bytes[8];
    is.read(bytes, 8);
    data = ((u64)bytes[7] << 56 & 0xff00000000000000) |
           ((u64)bytes[6] << 48 & 0x00ff000000000000) |
           ((u64)bytes[5] << 40 & 0x0000ff0000000000) |
           ((u64)bytes[4] << 32 & 0x000000ff00000000) |
           ((u64)bytes[3] << 24 & 0x00000000ff000000) |
           ((u64)bytes[2] << 16 & 0x0000000000ff0000) |
           ((u64)bytes[1] <<  8 & 0x000000000000ff00) |
           ((u64)bytes[0] <<  0 & 0x00000000000000ff);
    return is;
}

std::ifstream &operator>>(std::ifstream &is, s8 &data)
{
    // complicated in order to maintain pattern.
    byte bytes[1];
    is.read(bytes, 1);
    data = ((s8)bytes[0] << 0 & 0xff);
    return is;
}

std::ifstream &operator>>(std::ifstream &is, s16 &data)
{
    // read bytes into an array,
    // then restructure them into the data.
    // most significant byte at end.
    // bytes have to be cast to appropriate size,
    // then shifted, then a mask applied to make sure only
    // the 8 original bits are visible.
    byte bytes[2];
    is.read(bytes, 2);
    data = ((s16)bytes[1] << 8 & 0xff00) |
           ((s16)bytes[0] << 0 & 0x00ff);
    return is;
}

std::ifstream &operator>>(std::ifstream &is, s32 &data)
{
    // read bytes into an array,
    // then restructure them into the data.
    // most significant byte at end.
    byte bytes[4];
    is.read(bytes, 4);
    data = ((s32)bytes[3] << 24 & 0xff000000) |
           ((s32)bytes[2] << 16 & 0x00ff0000) |
           ((s32)bytes[1] <<  8 & 0x0000ff00) |
           ((s32)bytes[0] <<  0 & 0x000000ff);
    return is;
}

std::ifstream &operator>>(std::ifstream &is, s64 &data)
{
    // read bytes into an array,
    // then restructure them into the data.
    // most significant byte at end.
    byte bytes[8];
    is.read(bytes, 8);
    data = ((s64)bytes[7] << 56 & 0xff00000000000000) |
           ((s64)bytes[6] << 48 & 0x00ff000000000000) |
           ((s64)bytes[5] << 40 & 0x0000ff0000000000) |
           ((s64)bytes[4] << 32 & 0x000000ff00000000) |
           ((s64)bytes[3] << 24 & 0x00000000ff000000) |
           ((s64)bytes[2] << 16 & 0x0000000000ff0000) |
           ((s64)bytes[1] <<  8 & 0x000000000000ff00) |
           ((s64)bytes[0] <<  0 & 0x00000000000000ff);
    return is;
}


};
