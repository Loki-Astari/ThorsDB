
#ifndef THORS_ANVIL_MYSQL_PACKAGE_BUFFER_H
#define THORS_ANVIL_MYSQL_PACKAGE_BUFFER_H

#include "PackageStream.h"
#include <cstddef>

namespace ThorsAnvil
{
    namespace MySQL
    {

class PackageBufferMySQLDebugBuffer: public PackageStream
{
    PackageStream&    stream;
    public:
        PackageBufferMySQLDebugBuffer(PackageStream& stream)
            : stream(stream)
        {}
        virtual void readData(char* buffer, std::size_t len) override;
};

    }
}

#endif

