#include "PackageStream.h"
#include "RespPackage.h"
#include "ThorSQL/SQLUtil.h"

namespace ThorsAnvil
{
    namespace MySQL
    {

template<int len>
inline unsigned long long ConectReader::fixedLengthInteger()
{
    unsigned long long result = 0;
    THOR_MYSQL_READ_INT(result, len);

    return result;
}


    }
}
