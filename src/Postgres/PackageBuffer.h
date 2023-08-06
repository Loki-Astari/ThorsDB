#ifndef THORSANVIL_DB_POSTGRES_PACKAGE_BUFFER_H
#define THORSANVIL_DB_POSTGRES_PACKAGE_BUFFER_H

#include "PostgresConfig.h"
#include "ThorsDBCommon/StreamInterface.h"
#include <vector>
#include <string>
#include <algorithm>
#include <cstdint>
//#include <cstddef>   // for size_t (removed because it crashes clang 3.5 on travis

namespace ThorsAnvil::DB::Postgres
{

class PackageBuffer: public Common::StreamInterface
{
    Common::StreamInterface&    stream;
    bool                        messageOpen;
    std::size_t                 messageSize;
    std::size_t                 messageRead;
    std::vector<char>           sendBuffer;

    public:
        PackageBuffer(Common::StreamInterface& stream);
        virtual void        read(char* buffer, std::size_t len)         override;
        virtual void        write(char const* buffer, std::size_t len)  override;
        virtual bool        isEmpty()                                   override;
        virtual void        startNewConversation(bool reset)            override;
        virtual void        flush()                                     override;
        virtual void        drop()                                      override;
        virtual void        reset()                                     override;
        virtual std::string readRemainingData()                         override;
        virtual void        establishSSLConnection()                    override;

        void sendMessage(char type);
        char getMessage();

    private:
        char openMessage();

};

}

#endif
