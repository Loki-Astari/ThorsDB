
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
    T&              stream;
    std::size_t     currentPacketSize;
    std::size_t     currentPacketPosition;
    unsigned char   currentPacketSequenceID;
    bool            hasMore;

    private:
        void nextPacket();

    public:
        PackageBufferMySQLDebugBuffer(T& stream);
        virtual void        read(char* buffer, std::size_t len) override;
        virtual bool        isEmpty()                           override;
        virtual std::string readRemainingData()                 override;
};

    }
}

#ifndef COVERAGE_TEST
#include "PackageBuffer.tpp"
#endif

#endif

