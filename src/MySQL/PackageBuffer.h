
#ifndef THORS_ANVIL_MYSQL_PACKAGE_BUFFER_H
#define THORS_ANVIL_MYSQL_PACKAGE_BUFFER_H

#include "PackageStream.h"
#include <cstddef>

namespace ThorsAnvil
{
    namespace MySQL
    {

template<typename T>
class PackageBufferMySQLDebugBuffer: public PackageStream
{
    T&    stream;
    public:
        PackageBufferMySQLDebugBuffer(T& stream)
            : stream(stream)
        {}
        virtual void read(char* buffer, std::size_t len) override;
};

    }
}

#ifndef COVERAGE_TEST
#include "PackageBuffer.tpp"
#endif

#endif

