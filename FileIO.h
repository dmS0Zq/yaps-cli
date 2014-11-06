#ifndef FILEIO_H
#define FILEIO_H
#include <stdint.h>
#include <string>
#include <fstream>

namespace FileIO
{
// typedefs to make type names shorter
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;
typedef char byte;

//
// stores an arbitrarily long series of bytes.
// makes it easier to write longer things to file.
//
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

//
// overload stream operators to write out all the above types
// in the way this program expects
//
std::ofstream &operator<<(std::ofstream &os, LongData data);
std::ofstream &operator<<(std::ofstream &os, u8 data);
std::ofstream &operator<<(std::ofstream &os, u16 data);
std::ofstream &operator<<(std::ofstream &os, u32 data);
std::ofstream &operator<<(std::ofstream &os, u64 data);
std::ofstream &operator<<(std::ofstream &os, s8 data);
std::ofstream &operator<<(std::ofstream &os, s16 data);
std::ofstream &operator<<(std::ofstream &os, s32 data);
std::ofstream &operator<<(std::ofstream &os, s64 data);
std::ifstream &operator>>(std::ifstream &is, LongData &data);
std::ifstream &operator>>(std::ifstream &is, u8 &data);
std::ifstream &operator>>(std::ifstream &is, u16 &data);
std::ifstream &operator>>(std::ifstream &is, u32 &data);
std::ifstream &operator>>(std::ifstream &is, u64 &data);
std::ifstream &operator>>(std::ifstream &is, s8 &data);
std::ifstream &operator>>(std::ifstream &is, s16 &data);
std::ifstream &operator>>(std::ifstream &is, s32 &data);
std::ifstream &operator>>(std::ifstream &is, s64 &data);

}


#endif // FILEIO_H
