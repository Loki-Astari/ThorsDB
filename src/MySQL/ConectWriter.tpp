#ifndef THORS_ANVIL_DB_MYSQL_CONECT_WRITTER_TPP
#define THORS_ANVIL_DB_MYSQL_CONECT_WRITTER_TPP

#include "ThorSQL/StreamInterface.h"

namespace ThorsAnvil::DB::MySQL
{

template<int len>
void ConectWriter::writeFixedLengthInteger(unsigned long long value)
{
    THOR_MYSQL_WRITE_INT(value, len);
}

}

#endif
