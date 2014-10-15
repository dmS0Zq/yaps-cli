#ifndef DATETIME_H
#define DATETIME_H

#include <string>

namespace DateAndTime
{
    class DateTime
    {
    private:
        uint64_t value;
        bool    valid;

        bool checkValid();

        int year  () {return value % 100000000000000 / 10000000000;}
        int month () {return value % 10000000000     / 100000000  ;}
        int day   () {return value % 100000000       / 1000000    ;}
        int hour  () {return value % 1000000         / 10000      ;}
        int minute() {return value % 10000           / 100        ;}
        int second() {return value % 100             / 1          ;}

        static int64_t str_int64date(std::string str);
    public:
        void addYear  (int num = 1) {if (num < 0) subYear  (-num); else {                                                      value += num * 10000000000;}}
        void subYear  (int num = 1) {if (num < 0) addYear  (-num); else {if (num > year()) num = year();                       value -= num * 10000000000;}}
        void addMonth (int num = 1) {if (num < 0) subMonth (-num); else {while (month () + num > 12) {addYear  (); num -= 12;} value += num * 100000000;}}
        void subMonth (int num = 1) {if (num < 0) addMonth (-num); else {while (month () - num <  1) {subYear  (); num -= 12;} value -= num * 100000000;}}
        void addDay   (int num = 1); // this unfortunately requires some harder calculations
        void subDay   (int num = 1); // this unfortuantely requires some harder calculations
        void addHour  (int num = 1) {if (num < 0) subHour  (-num); else {while (hour  () + num > 23) {addDay   (); num -= 24;} value += num * 10000;}}
        void subHour  (int num = 1) {if (num < 0) addHour  (-num); else {while (hour  () - num <  0) {subDay   (); num -= 24;} value -= num * 10000;}}
        void addMinute(int num = 1) {if (num < 0) subMinute(-num); else {while (minute() + num > 59) {addHour  (); num -= 60;} value += num * 100;}}
        void subMinute(int num = 1) {if (num < 0) addMinute(-num); else {while (minute() - num <  0) {subHour  (); num -= 60;} value -= num * 100;}}
        void addSecond(int num = 1) {if (num < 0) subSecond(-num); else {while (second() + num > 59) {addMinute(); num -= 60;} value += num * 1;}}
        void subSecond(int num = 1) {if (num < 0) addSecond(-num); else {while (second() - num <  0) {subMinute(); num -= 60;} value -= num * 1;}}
        void modYear  (int num) {addYear  (num);}
        void modMonth (int num) {addMonth (num);}
        void modDay   (int num) {addDay   (num);}
        void modHour  (int num) {addHour  (num);}
        void modMinute(int num) {addMinute(num);}
        void modSecond(int num) {addSecond(num);}
        std::string format(std::string fStr);
        bool      isValid   ()              {return valid;}
        uint64_t  asUInt64  () const        {return value;}
        void      toUTC     (int timeZone)  {toTimeZone(-timeZone);}
        void      toTimeZone(int timeZone)  {modHour(timeZone);}

        DateTime(std::string str);
        DateTime(uint64_t datetime);
        DateTime();
    };

    bool operator< (const DateTime& a, const DateTime& b);
    bool operator> (const DateTime& a, const DateTime& b);
    bool operator<=(const DateTime& a, const DateTime& b);
    bool operator>=(const DateTime& a, const DateTime& b);
    bool operator==(const DateTime& a, const DateTime& b);
    bool operator!=(const DateTime& a, const DateTime& b);

    DateTime getCurrentLocalTime();
}
#endif // DATETIME_H

