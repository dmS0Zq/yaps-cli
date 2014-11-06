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
    enum GenerateMode
    {
        NORMAL_MODE     = 0,
        SPECIAL_MODE    = 1
    };
    void         setClassEnabled  (CharacterClass cc, bool enabled = true);
    bool         getClassEnabled  (CharacterClass cc);
    void         setClassDisabled (CharacterClass cc, bool disabled = true) {setClassEnabled(cc, !disabled);}
    bool         getClassDisabled (CharacterClass cc)                       {return !getClassEnabled(cc);}
    void         setClassMinimum  (CharacterClass cc, int min);
    int          getClassMinimum  (CharacterClass cc);
    void         setClassRequired (CharacterClass cc)                       {setClassMinimum(cc, 1);}
    bool         getClassRequired (CharacterClass cc)                       {return getClassMinimum(cc) >= 1;}
    void         setLength        (int length)                              {m_minimumLength = m_maximumLength = length;}
    void         setLength        (int minLen, int maxLen);
    unsigned int getMinLength     ()                                        {return m_minimumLength;}
    unsigned int getMaxLength     ()                                        {return m_maximumLength;}
    void         setMode          (GenerateMode gm)                         {m_mode = gm;}
    GenerateMode getMode          ()                                        {return m_mode;}
    void         setSpecialCharset(std::string charset)                     {m_specialCharset = charset;}
    std::string  getSpecialCharset()                                        {return m_specialCharset;}
    void         enableAllClasses ();
    void         disableAllClasses();
    void         requireAllClasses();
    unsigned int numberOfClasses  ()                                        {return CHAR_CLASS_SIZE;}
    std::string  generate         ();
    PasswordPolicy();
private:
    GenerateMode m_mode;
    unsigned int m_minimumLength;
    unsigned int m_maximumLength;
    std::vector<int> m_charClassMinimums;
    std::string m_specialCharset;
    static const std::vector<std::string> CHARSETS;
    static std::random_device rd;
    static std::mt19937_64 mtrand_64;
    unsigned int getSumOfClassMinimums();
    bool getAnyClassEnabled();
};

#endif // PASSWORDPOLICY_H
