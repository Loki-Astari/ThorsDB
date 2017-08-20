#ifndef THORS_ANVIL_MYSQL_PACKAGE_STREAM_H
#define THORS_ANVIL_MYSQL_PACKAGE_STREAM_H

#include <string>
//#include <cstddef>   // for size_t (removed because it crashes clang 3.5 on travis

namespace ThorsAnvil
{
    namespace MySQL
    {

class PackageStream
{
    public:
        virtual ~PackageStream()                                        = 0;
        virtual void        read(char* buffer, std::size_t len)         = 0;
        virtual void        write(char const* buffer, std::size_t len)  = 0;
        virtual void        startNewConversation(bool reset)            = 0;
        virtual void        flush()                                     = 0;
        virtual void        reset()                                     = 0;
        virtual void        drop()                                      = 0;
        virtual bool        isEmpty()                                   = 0;
        virtual std::string readRemainingData()                         = 0;
};

    }
}

#endif
