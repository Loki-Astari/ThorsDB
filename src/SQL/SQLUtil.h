#ifndef THORS_ANVIL_SQL_SQL_UTIL_H
#define THORS_ANVIL_SQL_SQL_UTIL_H

#include <map>
#include <string>
#include <sstream>

namespace ThorsAnvil
{
    namespace SQL
    {

using Options=std::map<std::string, std::string>;

    }

/* The following is generic to all ThorsAnvil Code */

/* Build error message or bug report for exceptions */
template<typename T>
void printItem(std::ostream& str, T const& msg)
{
    str << msg;
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
