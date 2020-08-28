#ifndef THORS_ANVIL_DB_MYSQL_PACKAGE_BUFFER_H
#define THORS_ANVIL_DB_MYSQL_PACKAGE_BUFFER_H

#include "ThorSQL/StreamInterface.h"
#include <vector>
#include <string>
//#include <cstddef>   // for size_t (removed because it crashes clang 3.5 on travis

namespace ThorsAnvil::DB::MySQL
{

class PackageBuffer: public SQL::StreamInterface
{
    SQL::StreamInterface& stream;
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
        PackageBuffer(SQL::StreamInterface& stream, bool flushed = false);
        virtual void        read(char* buffer, std::size_t len)         override;
        virtual void        write(char const* buffer, std::size_t len)  override;
        virtual bool        isEmpty()                                   override;
        virtual void        startNewConversation(bool reset)            override;
        virtual void        flush()                                     override;
        virtual void        drop()                                      override;
        virtual void        reset()                                     override;
        virtual std::string readRemainingData()                         override;
        virtual void        establishSSLConnection()                    override;
};

}

#endif
