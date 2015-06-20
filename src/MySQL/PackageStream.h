
#ifndef THORS_ANVIL_MYSQL_PACKAGE_STREAM_H
#define THORS_ANVIL_MYSQL_PACKAGE_STREAM_H

#include <string>

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
        virtual void        flush()                                     = 0;
        virtual bool        isEmpty()                                   = 0;
        virtual std::string readRemainingData()                         = 0;
};
inline PackageStream::~PackageStream() {}

class MySQLStream: public PackageStream
{
    static std::size_t constexpr ErrorResult = static_cast<std::size_t>(-1);

    int socket;
    public:
         MySQLStream(std::string const& host, int port);
        ~MySQLStream();
        virtual void        read(char* buffer, std::size_t len)         override;
        virtual void        write(char const* buffer, std::size_t len)  override;
        virtual void        flush()                                     override {}
        virtual bool        isEmpty()                                   override {return true;}
        virtual std::string readRemainingData()                         override {return "";}
};

    }
}

#endif

