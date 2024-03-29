#ifndef THORSANVIL_DB_COMMON_STREAM_INTERFACE_H
#define THORSANVIL_DB_COMMON_STREAM_INTERFACE_H

#include <string>
//#include <cstddef>   // for size_t (removed because it crashes clang 3.5 on travis

namespace ThorsAnvil::DB::Common
{

class StreamInterface
{
    public:
        virtual ~StreamInterface()                                      = 0;
        virtual void        read(char* buffer, std::size_t len)         = 0;
        virtual void        write(char const* buffer, std::size_t len)  = 0;
        virtual void        startNewConversation(bool reset)            = 0;
        virtual void        flush()                                     = 0;
        virtual void        reset()                                     = 0;
        virtual void        drop()                                      = 0;
        virtual bool        isEmpty()                                   = 0;
        virtual std::string readRemainingData()                         = 0;

        virtual void        establishSSLConnection()                    = 0;
};
inline StreamInterface::~StreamInterface() {}

}

#endif
