#ifndef THORS_ANVIL_SQL_STREAM_SIMPLE_H
#define THORS_ANVIL_SQL_STREAM_SIMPLE_H

#include "StreamInterface.h"
#include <string>
//#include <cstddef>   // for size_t (removed because it crashes clang 3.5 on travis

namespace ThorsAnvil
{
    namespace SQL
    {

class StreamSimple: public StreamInterface
{
    static std::size_t constexpr ErrorResult = static_cast<std::size_t>(-1);

    int socket;
    public:
         StreamSimple(std::string const& host, int port);
         StreamSimple(int socket);
        ~StreamSimple();
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
