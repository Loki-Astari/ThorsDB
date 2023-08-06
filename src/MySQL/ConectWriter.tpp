#ifndef THORSANVIL_DB_MYSQL_CONECT_WRITTER_TPP
#define THORSANVIL_DB_MYSQL_CONECT_WRITTER_TPP

#include "ThorsDBCommon/StreamInterface.h"

namespace ThorsAnvil::DB::MySQL
{

template<int len>
void ConectWriter::writeFixedLengthInteger(unsigned long long value)
{
    THOR_MYSQL_WRITE_INT(value, len);
}

}

#endif
