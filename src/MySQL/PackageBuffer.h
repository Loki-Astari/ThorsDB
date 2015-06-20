
#ifndef THORS_ANVIL_MYSQL_PACKAGE_BUFFER_H
#define THORS_ANVIL_MYSQL_PACKAGE_BUFFER_H

#include "PackageStream.h"
#include <vector>
#include <cstddef>

namespace ThorsAnvil
{
    namespace MySQL
    {

template<typename T>
class PackageBufferMySQLDebugBuffer: public PackageStream
{
    T&                  stream;
    std::size_t         readCurrentPacketSize;
    std::size_t         readCurrentPacketPosition;
    unsigned char       readCurrentPacketSequenceID;
    unsigned char       writCurrentPacketSequenceID;
    bool                hasMore;
    std::vector<char>   sendBuffer;

    private:
        void nextPacket();
        void writePackageHeader(std::size_t size);
        void writeStream(char const* buffer, std::size_t len);

    public:
        PackageBufferMySQLDebugBuffer(T& stream);
        virtual void        read(char* buffer, std::size_t len)         override;
        virtual void        write(char const* buffer, std::size_t len)  override;
        virtual bool        isEmpty()                                   override;
        virtual void        flush()                                     override;
        virtual std::string readRemainingData()                         override;
};

    }
}

#ifndef COVERAGE_TEST
#include "PackageBuffer.tpp"
#endif

#endif

