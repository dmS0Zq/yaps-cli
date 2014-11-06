#include "PasswordPolicy.h"
#include <algorithm>

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
    m_mode = NORMAL_MODE;
    m_specialCharset = "";
    m_charClassMinimums = std::vector<int>(CHAR_CLASS_SIZE, -1);
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

void PasswordPolicy::enableAllClasses()
{
    for (unsigned int cc = 0; cc < CHAR_CLASS_SIZE; ++cc)
        setClassEnabled(static_cast<CharacterClass>(cc));
}

void PasswordPolicy::disableAllClasses()
{
    for (unsigned int cc = 0; cc < CHAR_CLASS_SIZE; ++cc)
        setClassDisabled(static_cast<CharacterClass>(cc));
}

void PasswordPolicy::requireAllClasses()
{
    for (unsigned int cc = 0; cc < CHAR_CLASS_SIZE; ++cc)
        setClassRequired(static_cast<CharacterClass>(cc));
}

std::string PasswordPolicy::generate()
{
    if (m_mode == NORMAL_MODE)
    {
        unsigned int minCharsRequried = getSumOfClassMinimums();
        if (minCharsRequried > m_maximumLength)
            throw InvalidInternalStateException();
        else if (!getAnyClassEnabled())
            throw InvalidInternalStateException();

        std::vector<char> selectedChars;
        std::uniform_int_distribution<unsigned int> distSelectedSize(std::max(minCharsRequried, m_minimumLength), m_maximumLength);
        unsigned int numChars = distSelectedSize(mtrand_64);
        selectedChars.reserve(numChars);
        // make distributions for randomly generating chars
        std::vector<std::uniform_int_distribution<unsigned int>> dists;
        for (unsigned int cc = 0; cc < CHAR_CLASS_SIZE; ++cc)
            dists.push_back(std::uniform_int_distribution<unsigned int>(0, CHARSETS[cc].length()-1));
        // add randomly selected chars to selected chars vector
        for (unsigned int cc = 0; cc < CHAR_CLASS_SIZE; ++cc)
            for (int i = 0; i < getClassMinimum(static_cast<CharacterClass>(cc)); ++i)
                selectedChars.push_back(CHARSETS[cc].at(dists[cc](mtrand_64)));

        if (selectedChars.size() < selectedChars.capacity())
        {
            std::string extraSet = "";
            for (unsigned int cc = 0; cc < CHAR_CLASS_SIZE; ++cc)
                if (getClassEnabled(static_cast<CharacterClass>(cc)))
                    extraSet += CHARSETS[cc];
            std::uniform_int_distribution<unsigned int> distExtraSet(0, extraSet.length()-1);
            while (selectedChars.size() < selectedChars.capacity())
            {
                selectedChars.push_back(extraSet[distExtraSet(mtrand_64)]);
            }
        }
        std::shuffle(selectedChars.begin(), selectedChars.end(), mtrand_64);
        std::string pw = "";
        for (unsigned int i = 0; i < selectedChars.size(); i++)
            pw += selectedChars[i];
        return pw;
    }
    else if (m_mode == SPECIAL_MODE)
    {
        if (m_specialCharset.length() == 0)
            throw InvalidInternalStateException();

        std::vector<char> selectedChars;
        std::uniform_int_distribution<unsigned int> distSize(m_minimumLength, m_maximumLength);
        unsigned int numChars = distSize(mtrand_64);
        std::uniform_int_distribution<unsigned int> distSpecialCharset(0, m_specialCharset.length()-1);
        for (unsigned int i = 0; i < numChars; i++)
            selectedChars.push_back(m_specialCharset.at(distSpecialCharset(mtrand_64)));
        std::shuffle(selectedChars.begin(), selectedChars.end(), mtrand_64);
        std::string pw = "";
        for (unsigned int i = 0; i < selectedChars.size(); i++)
            pw += selectedChars[i];
        return pw;
    }
    else
        throw InvalidInternalStateException();
}

unsigned int PasswordPolicy::getSumOfClassMinimums()
{
    unsigned int val = 0;
    for (unsigned int cc = 0; cc < CHAR_CLASS_SIZE; ++cc)
        if (getClassRequired(static_cast<CharacterClass>(cc)))
            val += getClassMinimum(static_cast<CharacterClass>(cc));
    return val;
}

bool PasswordPolicy::getAnyClassEnabled()
{
    for (unsigned int cc = 0; cc < CHAR_CLASS_SIZE; ++cc)
        if (getClassEnabled(static_cast<CharacterClass>(cc)))
            return true;
    return false;
}
