#include "DateTime.h"
#include <sstream>
#include <cmath>
#include <ctime>

namespace NumStr
{
    // only works for positive unsigned int powers!
    int64_t pow(int64_t base, unsigned int exp)
    {
        if (exp > 0) return base * pow(base, exp-1);
        else return 1;
    }

    std::string int_str(int64_t val, unsigned int decimals,
                                unsigned int minDisplayDecimals, unsigned int maxDisplayDecimals,
                                unsigned int minDisplayWholes)
    {
        //if (maxDisplayDecimals < minDisplayDecimals) throw rangeException;
        std::stringstream ss;
        bool isPositive = (val >= 0);
        int64_t factor = pow(10,decimals); // my custom power function for integers required
        int64_t wholePart = std::abs(val / factor);
        int64_t fractPart = std::abs(val % factor);
        ss << wholePart;
        if (factor > 0 && fractPart > 0)
        {
            ss << ".";
            double numLeadingZeros = log10((double)factor/fractPart) - 1;
            while (numLeadingZeros-- > 0) {ss << "0";}
            ss << fractPart;
        }
        std::string str = ss.str();
    // work with rhs
        if (minDisplayDecimals > 0 && // want decimals no matter what
            str.find(".") != std::string::npos && // there are some already
            str.substr(str.find(".")+1, str.length()-1).length() < minDisplayDecimals) // and there isn't enough
        {
            str = str.substr(0, str.find(".")) + // lhs
                  "." + // decimal
                  str.substr(str.find(".")+1) + // rhs
                  std::string(minDisplayDecimals - str.substr(str.find(".")+1).length(), '0'); // extra zeros
        }
        else if (minDisplayDecimals > 0 && // want decimals no matter what
                 str.find(".") == std::string::npos) // there are not any yet
        {
            str += "." + std::string(minDisplayDecimals, '0');
        }
        if (str.find(".") != std::string::npos && // there is a rhs
            str.substr(str.find(".")+1).length() > maxDisplayDecimals) // rhs too long
        {
            str = str.substr(0, str.find(".")) + // lhs
                  "." + // decimal
                  str.substr(str.find(".")+1, maxDisplayDecimals); // rhs truncated
        }
    // work with lhs
        if (str.substr(0, str.find(".")).length() < minDisplayWholes) // not enough lhs digits
        {
            str = std::string(minDisplayWholes - str.substr(0, str.find(".")).length(), '0')  + str;
        }
        else if (str.substr(0, str.find(".")) == "0" && minDisplayWholes == 0) // don't want leading zeros for val < 1
        {
            str = str.substr(str.find("."));
        }
        if (!isPositive) str = "-" + str;
        return str;
    }

    int64_t str_int(std::string val)
    {
        if (val.find_first_not_of("0123456789-") != std::string::npos)
            {return 0;} // invalid character anywhere
        if (val.substr(1).find_first_not_of("0123456789") != std::string::npos)
            {return 0;} // negative sign somewhere other than front
        bool isPositive = true;
        if (val.at(0) == '-') {isPositive = false; val = val.substr(1);}
        int64_t value = 0;
        int pos = val.length() - 1;
        int64_t factor = 1;
        while (pos >= 0)
        {
            switch (val.at(pos))
            {
                case '1': value += 1*factor; break; case '2': value += 2*factor; break;
                case '3': value += 3*factor; break; case '4': value += 4*factor; break;
                case '5': value += 5*factor; break; case '6': value += 6*factor; break;
                case '7': value += 7*factor; break; case '8': value += 8*factor; break;
                case '9': value += 9*factor; break; case '0': value += 0*factor; break;
                //default: return 0; // non-numeric character found
            };
            pos--;
            factor *= 10;
        }
        if (!isPositive) {value *= -1;}
        return value;
    }
}

namespace DateAndTime
{

    int64_t DateTime::str_int64date(std::string str)
    {

        int64_t date;

        if (str.length() == 8) // YYYYMMDD hopefully
        {
            // definitely YYYYMMDD
            if (str.find_first_not_of("0123456789") == std::string::npos)
            {
                date =
                    NumStr::str_int(str.substr(0,4)) * 10000000000 +
                    NumStr::str_int(str.substr(4,2)) * 100000000 +
                    NumStr::str_int(str.substr(6,2)) * 1000000;
            }
            else date = 0;
        }
        else if (str.length() == 10) // YYYY-MM-DD or similar hopefully
        {
            // definitely YYYY-MM-DD or similar
            if (str.at(4) == str.at(7))
            {
                date =
                    NumStr::str_int(str.substr(0,4)) * 10000000000 +
                    NumStr::str_int(str.substr(5,2)) * 100000000 +
                    NumStr::str_int(str.substr(8,2)) * 1000000;
            }
            else date = 0;
        }
        else if (str.length() == 14) // YYYYMMDDHHMMSS hopefully
        {
            // definitely YYYYMMDDHHMMSS
            if (str.find_first_not_of("0123456789") == std::string::npos)
            {
                date =
                    NumStr::str_int(str.substr(0,4))  * 10000000000 +
                    NumStr::str_int(str.substr(4,2))  * 100000000 +
                    NumStr::str_int(str.substr(6,2))  * 1000000 +
                    NumStr::str_int(str.substr(8,2))  * 10000 +
                    NumStr::str_int(str.substr(10,2)) * 100 +
                    NumStr::str_int(str.substr(12,2)) * 1;
            }
            else date = 0;
        }
        else if (str.length() == 19) // YYYY-MM-DD HH:MM:SS or similar hopefully
        {
            // definitely YYYY-MM-DD HH:MM:SS or similar
            if (str.at(4)==str.at(7) && str.at(13)==str.at(16))
            {
                date =
                    NumStr::str_int(str.substr(0,4))  * 10000000000 +
                    NumStr::str_int(str.substr(5,2))  * 100000000 +
                    NumStr::str_int(str.substr(8,2))  * 1000000 +
                    NumStr::str_int(str.substr(11,2)) * 10000 +
                    NumStr::str_int(str.substr(14,2)) * 100 +
                    NumStr::str_int(str.substr(17,2)) * 1;
            }
            else date = 0;
        }
        else date = 0;
        return date;
    }

    bool DateTime::checkValid()
    {
        if (year() < 1) return false;
        if      (year() > 9999 || year() < 1) return false;
        else if (month() > 12 || month() < 1) return false;
        else if (day() > 31 || day () < 1) return false; // still needs follow up
        else if (hour()   > 23) return false;
        else if (minute() > 59) return false;
        else if (second() > 59) return false;
        switch (month()) // day follow up
        {
        // 31-day months already taken care of
        // 30-day months
        case 4:case 6:case 9:case 11:
            if (day() > 30) return false;
            break;
        // 28-to-29-day month
        case 2:
            if (day() > 29) return false; // trash obvious bad dates
            else if (day() == 29) // if 29, make sure leap year
            {
                if (year() % 4 != 0) return false;
                else if (year() % 100 == 0 && year() % 400 != 0) return false;
            }
            break;
        }
        return true;
    }

    void DateTime::addDay(int num)
    {
        if (num < 0) subDay(-num);
        else
        {
            switch(month())
            {
            case 1:case 3:case 5:case 7:case 8:case 10:case 12:
                if (day() + num >  31) {addMonth(); num -= 31; addDay(num);} else {value += num * 1000000;} break;
            case 4:case 6:case 9:case 11:
                if (day() + num >  30) {addMonth(); num -= 30; addDay(num);} else {value += num * 1000000;} break;
            case 2: // leap years, man ...
                bool leapYear = false;
                if (year() % 4 == 0)
                {
                    if      (year() % 100 != 0) leapYear = true;
                    else if (year() % 400 == 0) leapYear = true;
                    else leapYear = false;
                }
                else leapYear = false;
                if (day() + num > 29)
                {
                    if (leapYear) num -= 29;
                    else          num -= 28;
                    addMonth();
                    addDay(num);
                }
                else if (day() + num == 29)
                {
                    if (leapYear)
                    {
                        value += num * 1000000;
                    }
                    else
                    {
                        num -= 28;
                        addMonth();
                        addDay(num);
                    }
                }
                else
                {
                    value += num * 1000000;
                }
                break;
            }
        }
    }

    void DateTime::subDay(int num)
    {
        if (num < 0) addDay(-num);
        else
        {
            switch (month())
            {
            case 2:case 4:case 6: case 8:case 9:case 11:case 1: // each are 1 month after 31-day months
                if (day() - num < 1) {subMonth(); num -= 31; subDay(num);} else {value -= num * 1000000;} break;
            case 5:case 7:case 10:case 12: // each are 1 month after 30-day months
                if (day() - num < 1) {subMonth(); num -= 30; subDay(num);} else {value -= num * 1000000;} break;
            case 3:
                bool leapYear = false;
                if (year() % 4 == 0)
                {
                    if      (year() % 100 != 0) leapYear = true;
                    else if (year() % 400 == 0) leapYear = true;
                    else leapYear = false;
                }
                else leapYear = false;
                if (day() - num < 1)
                {
                    if (leapYear) num -= 29;
                    else          num -= 28;
                    subMonth();
                    subDay(num);
                }
                else
                {
                    value -= num * 1000000;
                }
                break;
            }
        }
    }

    std::string DateTime::format(std::string fStr)
    {
        std::string::size_type pos = fStr.find("%Y");
        if (pos != std::string::npos)
            return format(fStr.replace(pos, 2, NumStr::int_str(year(),0,0,0,4)));

        pos = fStr.find("%M");
        if (pos != std::string::npos)
            return format(fStr.replace(pos, 2, NumStr::int_str(month(),0,0,0,2)));

        pos = fStr.find("%D");
        if (pos != std::string::npos)
            return format(fStr.replace(pos, 2, NumStr::int_str(day(),0,0,0,2)));

        pos = fStr.find("%H");
        if (pos != std::string::npos)
            return format(fStr.replace(pos, 2, NumStr::int_str(hour(),0,0,0,2)));

        pos = fStr.find("%I");
        if (pos != std::string::npos)
            return format(fStr.replace(pos, 2, NumStr::int_str(minute(),0,0,0,2)));

        pos = fStr.find("%S");
        if (pos != std::string::npos)
            return format(fStr.replace(pos, 2, NumStr::int_str(second(),0,0,0,2)));

        return fStr;
    }

    DateTime::DateTime(std::string str)
    {
        value = str_int64date(str);
        valid = checkValid();
    }

    DateTime::DateTime(uint64_t datetime)
    {
        value = datetime;
        valid = checkValid();
    }

    DateTime::DateTime() {}

    bool operator< (const DateTime& a, const DateTime& b) {return a.asUInt64() < b.asUInt64();}
    bool operator> (const DateTime& a, const DateTime& b) {return   b < a ;}
    bool operator<=(const DateTime& a, const DateTime& b) {return !(a > b);}
    bool operator>=(const DateTime& a, const DateTime& b) {return !(a < b);}
    bool operator==(const DateTime& a, const DateTime& b) {return a.asUInt64() == b.asUInt64();}
    bool operator!=(const DateTime& a, const DateTime& b) {return !(a == b);}

    DateTime getCurrentLocalTime()
    {
        time_t t = time(0);
        struct tm* timeinfo = localtime(&t);
        char buf[80];
        strftime(buf, 80, "%F %T", timeinfo);
        return DateTime(std::string(buf));
    }

}
