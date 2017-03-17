#ifndef THORS_ANVIL_MYSQL_PACKAGE_BUFFER_H
#define THORS_ANVIL_MYSQL_PACKAGE_BUFFER_H

#include "PackageStream.h"
#include <vector>
#include <string>
//#include <cstddef>   // for size_t (removed because it crashes clang 3.5 on travis

namespace ThorsAnvil
{
    namespace MySQL
    {

template<typename T>
class PackageBuffer: public PackageStream
{
    T&                  stream;
    std::size_t         readCurrentPacketSize;
    std::size_t         readCurrentPacketPosition;
    unsigned char       currentPacketSequenceID;
    bool                hasMore;
    bool                flushed;
    std::vector<char>   sendBuffer;

    private:
        void nextPacket();
        void writePackageHeader(std::size_t size);
        void writeStream(char const* buffer, std::size_t len);

    public:
        PackageBuffer(T& stream, bool flushed = false);
        virtual void        read(char* buffer, std::size_t len)         override;
        virtual void        write(char const* buffer, std::size_t len)  override;
        virtual bool        isEmpty()                                   override;
        virtual void        startNewConversation()                      override;
        virtual void        flush()                                     override;
        virtual void        drop()                                      override;
        virtual void        reset()                                     override;
        virtual std::string readRemainingData()                         override;
};

    }
}

#ifndef COVERAGE_MySQL
#include "PackageBuffer.tpp"
#endif

#endif
