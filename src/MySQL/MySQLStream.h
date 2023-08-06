#ifndef THORSANVIL_DB_MYSQL_MYSQL_STREAM_H
#define THORSANVIL_DB_MYSQL_MYSQL_STREAM_H

#include "ThorsDBCommon/StreamSimple.h"

namespace ThorsAnvil::DB::MySQL
{

class MySQLStream: public Common::StreamSimple
{
    public:
        MySQLStream(int fd)
            : Common::StreamSimple(fd)
        {}
        MySQLStream(std::string const& host, int port = 3306)
            : Common::StreamSimple(host, port ? port : 3306)
        {}
};

}

#endif
