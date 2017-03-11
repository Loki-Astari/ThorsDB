#include "ThorMySQL.h"
#include "PackageStream.h"
#include <stdexcept>
#include <typeinfo>

namespace ThorsAnvil
{
    namespace MySQL
    {

template<int len>
void ConectWriter::writeFixedLengthInteger(unsigned long long value)
{
    THOR_MYSQL_WRITE_INT(value, len);
}

    }
}
