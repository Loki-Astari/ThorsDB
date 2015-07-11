
#ifndef THORS_ANVIL_SQL_SQL_UTIL_H
#define THORS_ANVIL_SQL_SQL_UTIL_H

#include <map>
#include <string>

namespace ThorsAnvil
{
    namespace SQL
    {

using Options=std::map<std::string, std::string>;
enum  StatementType {Prepare, OneTime};

/* Totally generic platform independent way of marking unused variables */
template<typename T>
inline void thorUnused(T const&)
{}

    }
}

#endif

