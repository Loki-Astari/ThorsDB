#ifndef THORS_ANVIL_MYSQL_MY_SQL_STREAM_H
#define THORS_ANVIL_MYSQL_MY_SQL_STREAM_H

#include "PackageStream.h"
#include <string>
#include <cstddef>

namespace ThorsAnvil
{
    namespace MySQL
    {

class MySQLStream: public PackageStream
{
    static std::size_t constexpr ErrorResult = static_cast<std::size_t>(-1);

    int socket;
    public:
         MySQLStream(std::string const& host, int port);
         MySQLStream(int socket);
        ~MySQLStream();
        virtual void        read(char* buffer, std::size_t len)         override;
        virtual void        write(char const* buffer, std::size_t len)  override;
        virtual void        startNewConversation()                      override {}
        virtual void        flush()                                     override {}
        virtual void        reset()                                     override {}
        virtual void        drop()                                      override {}
        virtual bool        isEmpty()                                   override {return true;}
        virtual std::string readRemainingData()                         override {return "";}
};

    }
}

#endif
