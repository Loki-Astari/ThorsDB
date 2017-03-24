#ifndef THORS_ANVIL_SQL_STREAM_SIMPLE_H
#define THORS_ANVIL_SQL_STREAM_SIMPLE_H

#include "StreamInterface.h"
#include <string>
#include <functional>
//#include <cstddef>   // for size_t (removed because it crashes clang 3.5 on travis

static std::size_t constexpr ErrorResult = static_cast<std::size_t>(-1);

namespace ThorsAnvil
{
    namespace SQL
    {

class StreamSimple: public StreamInterface
{
    int socket;
    std::function<void()>  readYield;
    std::function<void()>  writeYield;
    public:
         StreamSimple(std::string const& host, int port, bool nonBlocking = false);
         StreamSimple(int socket);
        ~StreamSimple();
        void close();

        virtual void        read(char* buffer, std::size_t len)         override;
        virtual void        write(char const* buffer, std::size_t len)  override;
        virtual void        startNewConversation(bool)                  override {}
        virtual void        flush()                                     override {}
        virtual void        reset()                                     override {}
        virtual void        drop()                                      override {}
        virtual bool        isEmpty()                                   override {return true;}
        virtual std::string readRemainingData()                         override {return "";}

        int getSocketId() const {return socket;}
        void setYield(std::function<void()>&& yr, std::function<void()>&& yw)
        {
            readYield = std::move(yr);
            writeYield= std::move(yw);
        }
};

    }
}

#endif
