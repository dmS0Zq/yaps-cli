#ifndef PASSWORDPOLICY_H
#define PASSWORDPOLICY_H

#include <vector>
#include <string>
#include "Exceptions.h"

class PasswordPolicy
{
public:
    enum CharacterClass
    {
        CHAR_CLASS_LOW_ALPHAS = 0,
        CHAR_CLASS_UPPER_ALPHAS = 1,
        CHAR_CLASS_DIGITS = 2,
        CHAR_CLASS_SYMBOLS = 3,
        CHAR_CLASS_SPACE = 4,
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
    std::string generate() {return "";}
    PasswordPolicy();
private:
    unsigned int m_minimumLength;
    unsigned int m_maximumLength;
    std::vector<int> m_charClassMinimums;
};

#endif // PASSWORDPOLICY_H
