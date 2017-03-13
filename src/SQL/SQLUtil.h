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
enum StatementType {Prepare, OneTime};

    }

/* The following is generic to all ThorsAnvil Code */

/* Totally generic platform independent way of marking unused variables */
template<typename T>
inline void thorUnused(T const&)
{}

/* Build error message or bug report for exceptions */
template<typename T>
int printItem(std::ostream& str, T const& msg)
{
    str << msg;
    return 1;
}
template<typename... Args>
std::string errorMsg(Args const& ...a)
{
    std::stringstream msg;
    auto list = {printItem(msg, a)...};
    thorUnused(list);

    return msg.str();
}
template<typename... Args>
std::string bugReport(Args const& ...a)
{
    return errorMsg(a..., "\nPlease File a Bug Report: ");
}

}

#endif
