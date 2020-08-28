#ifndef THORS_ANVIL_POSTG_PACKAGE_BUFFER_H
#define THORS_ANVIL_POSTG_PACKAGE_BUFFER_H

#include "ThorSQL/StreamInterface.h"
#include <vector>
#include <string>
#include <algorithm>
//#include <cstddef>   // for size_t (removed because it crashes clang 3.5 on travis

namespace ThorsAnvil::DB::Postgres
{

class PackageBuffer: public SQL::StreamInterface
{
    SQL::StreamInterface&      stream;
    bool                messageOpen;
    std::size_t         messageSize;
    std::size_t         messageRead;
    std::vector<char>   sendBuffer;

    public:
        PackageBuffer(SQL::StreamInterface& stream);
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
