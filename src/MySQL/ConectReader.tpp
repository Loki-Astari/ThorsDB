#ifndef THORSANVIL_DB_MYSQL_CONECT_READER_TPP
#define THORSANVIL_DB_MYSQL_CONECT_READER_TPP

#include "ThorsDBCommon/StreamInterface.h"

namespace ThorsAnvil::DB::MySQL
{

template<int len>
inline unsigned long long ConectReader::fixedLengthInteger()
{
    unsigned long long result = 0;
    THOR_MYSQL_READ_INT(result, len);

    return result;
}


}

#endif
