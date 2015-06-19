
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
        virtual ~PackageStream()                            = 0;
        virtual void read(char* buffer, std::size_t len)    = 0;
};
inline PackageStream::~PackageStream() {}

class MySQLStream: public PackageStream
{
    int socket;
    public:
         MySQLStream(std::string const& host, int port);
        ~MySQLStream();
        virtual void read(char* buffer, std::size_t len) override;
};

    }
}

#endif

