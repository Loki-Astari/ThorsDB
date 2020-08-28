#include "ThorSQL/StreamInterface.h"

namespace ThorsAnvil::DB::MySQL
{

template<int len>
void ConectWriter::writeFixedLengthInteger(unsigned long long value)
{
    THOR_MYSQL_WRITE_INT(value, len);
}

}
