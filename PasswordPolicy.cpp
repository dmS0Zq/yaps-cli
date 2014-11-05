#include "PasswordPolicy.h"

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
