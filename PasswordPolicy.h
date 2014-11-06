#ifndef PASSWORDPOLICY_H
#define PASSWORDPOLICY_H

#include <vector>
#include <string>
#include <random>
#include <chrono>
#include "Exceptions.h"

class PasswordPolicy
{
public:
    enum CharacterClass
    {
        LOW_ALPHAS      = 0,
        UPPER_ALPHAS    = 1,
        DIGITS          = 2,
        SYMBOLS         = 3,
        SPACE           = 4,
        CHAR_CLASS_SIZE = 5
    };
    void enableClass(CharacterClass cc);
    void setClassEnabled(CharacterClass cc, bool enabled);
    void setClassDisabled(CharacterClass cc, bool disabled) {setClassEnabled(cc, !disabled);}
    bool getClassEnabled(CharacterClass cc);
    bool getClassDisabled(CharacterClass cc) {return !getClassEnabled(cc);}
    void setClassMinimum(CharacterClass cc, int min);
    int getClassMinimum(CharacterClass cc);
    void setLength(int length) {m_minimumLength = m_maximumLength = length;}
    void setLength(int minLen, int maxLen);
    unsigned int getMinLength() {return m_minimumLength;}
    unsigned int getMaxLength() {return m_maximumLength;}
    std::string generate();
    PasswordPolicy();
private:
    unsigned int m_minimumLength;
    unsigned int m_maximumLength;
    std::vector<int> m_charClassMinimums;
    static const std::vector<std::string> CHARSETS;
    static std::random_device rd;
    static std::mt19937_64 mtrand_64;
};

#endif // PASSWORDPOLICY_H
