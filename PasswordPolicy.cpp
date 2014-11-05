#include "PasswordPolicy.h"

#include <iostream>

const std::vector<std::string> PasswordPolicy::CHARSETS = { "abcdefghijklmnopqrstuvwxyz",
                                                            "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
                                                            "0123456789",
                                                            "`~!@#$%^&*()_-+={}[]\\|:;\"'<>,.?/",
                                                            " "};
std::random_device PasswordPolicy::rd;
std::mt19937_64 PasswordPolicy::mtrand_64 = std::mt19937_64(rd());

PasswordPolicy::PasswordPolicy()
{
    m_minimumLength = 0;
    m_maximumLength = 0;
    m_charClassMinimums = std::vector<int>(CHAR_CLASS_SIZE, -1);
}

void PasswordPolicy::enableClass(CharacterClass cc)
{
    if (cc >= CHAR_CLASS_SIZE)
        throw IndexOutOfRangeException();
    else if (m_charClassMinimums[cc] < 0)
        m_charClassMinimums[cc] = 0;
}

void PasswordPolicy::setClassEnabled(CharacterClass cc, bool enabled)
{
    if (cc >= CHAR_CLASS_SIZE)
        throw IndexOutOfRangeException();
    else if (enabled && m_charClassMinimums[cc] < 0)
        m_charClassMinimums[cc] = 0;
    else if (!enabled)
        m_charClassMinimums[cc] = -1;
}

bool PasswordPolicy::getClassEnabled(CharacterClass cc)
{
    if (cc >= CHAR_CLASS_SIZE)
        throw IndexOutOfRangeException();
    else
        return (m_charClassMinimums[cc] >= 0);
}

void PasswordPolicy::setClassMinimum(CharacterClass cc, int min)
{
    if (cc >= CHAR_CLASS_SIZE)
        throw IndexOutOfRangeException();
    else
        m_charClassMinimums[cc] = min;
}

int PasswordPolicy::getClassMinimum(CharacterClass cc)
{
    if (cc >= CHAR_CLASS_SIZE)
        throw IndexOutOfRangeException();
    else
        return m_charClassMinimums[cc];
}

void PasswordPolicy::setLength(int minLen, int maxLen)
{
    if (maxLen < minLen)
        throw InvalidArgumentException();
    else if (minLen < 0 || maxLen < 0)
        throw InvalidArgumentException();
    else
    {
        m_minimumLength = minLen;
        m_maximumLength = maxLen;
    }
}

std::string PasswordPolicy::generate()
{
    std::vector<char> selectedChars;
    std::uniform_int_distribution<unsigned int> distSelectedSize(m_minimumLength, m_maximumLength);
    std::vector<std::uniform_int_distribution<unsigned int>> dists;
    for (unsigned int cc = 0; cc < CHAR_CLASS_SIZE; ++cc)
        dists.push_back(std::uniform_int_distribution<unsigned int>(0, CHARSETS[cc].length()-1));
    unsigned int numChars = distSelectedSize(mtrand_64);
    selectedChars.reserve(numChars);

    for (unsigned int cc = 0; cc < CHAR_CLASS_SIZE; ++cc)
    {
        for (int i = 0; i < getClassMinimum(static_cast<CharacterClass>(cc)); ++i)
        {
            selectedChars.push_back(CHARSETS[cc].at(dists[cc](mtrand_64)));
        }
    }

    std::string pw = "";

    for (unsigned int i = 0; i < selectedChars.size(); i++)
        pw += selectedChars[i];
    return pw;
}
