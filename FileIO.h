#ifndef FILEIO_H
#define FILEIO_H
#include <stdint.h>
#include <string>
#include <fstream>

namespace FileIO
{
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;
typedef char byte;

class LongData
{
public:
    byte* m_data;
    u32 m_length;
public:
    std::string toString();
    LongData(std::string str);
    LongData(u32 length = 0, byte* data = nullptr);
    ~LongData();
};

std::ofstream& operator<<(std::ofstream &os, LongData data);
std::ofstream& operator<<(std::ofstream &os, uint8_t data);
std::ofstream& operator<<(std::ofstream &os, uint16_t data);
std::ofstream& operator<<(std::ofstream &os, uint32_t data);
std::ofstream& operator<<(std::ofstream &os, uint64_t data);

std::ifstream& operator>>(std::ifstream &is, LongData &data);
std::ifstream& operator>>(std::ifstream &is, uint8_t &data);
std::ifstream& operator>>(std::ifstream &is, uint16_t &data);
std::ifstream& operator>>(std::ifstream &is, uint32_t &data);
std::ifstream& operator>>(std::ifstream &is, uint64_t &data);

}


#endif // FILEIO_H
