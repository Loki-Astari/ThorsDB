#ifndef THORS_ANVIL_MYSQL_MYSQL_TIME_BAG_H
#define THORS_ANVIL_MYSQL_MYSQL_TIME_BAG_H

namespace ThorsAnvil
{
    namespace MySQL
    {

struct MySQLTimeBag
{
    enum TimeType {Absolute, RelativeNegative, RelativePositive};

    TimeType             type;
    unsigned long        year;
    unsigned long        month;
    unsigned long        day;
    unsigned long        hour;
    unsigned long        minute;
    unsigned long        second;
    unsigned long        uSecond;
    MySQLTimeBag()
        : type(Absolute), year(0), month(0), day(0), hour(0), minute(0), second(0), uSecond(0)
    {}
};

    }
}

#endif
