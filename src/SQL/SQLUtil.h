#ifndef THORS_ANVIL_DB_SQL_SQL_UTIL_H
#define THORS_ANVIL_DB_SQL_SQL_UTIL_H

#include <map>
#include <string>
#include <sstream>

namespace ThorsAnvil::DB
{
    namespace SQL
    {

using Options=std::map<std::string, std::string>;

    }

/* The following is generic to all ThorsAnvil Code */

/* Build error message or bug report for exceptions */

template<typename T>
struct ItemPrinter
{
    std::ostream& str;
    ItemPrinter(std::ostream& str) : str(str) {}
    void print(T const& msg){str << msg;}
};
template<typename P>
struct ItemPrinter<P*>
{
    std::ostream& str;
    ItemPrinter(std::ostream& str) : str(str) {}
    void print(P* const& msg)
    {
        if (msg)
        {
            str << (*msg);
        }
    }
};

template<typename T>
void printItem(std::ostream& str, T const& msg)
{
    ItemPrinter<T>  printer(str);
    printer.print(msg);
}

template<typename... Args>
std::string stringBuild(Args const& ...a)
{
    std::stringstream msg;
    auto list = {0, (printItem(msg, a), 0) ...};
    (void)list;

    return msg.str();
}

template<typename... Args>
std::string errorMsg(Args const& ...a)
{
    return stringBuild(a...);
}

template<typename... Args>
std::string bugReport(Args const& ...a)
{
    return stringBuild(a..., "\nPlease File a Bug Report: ");
}

}

#endif
