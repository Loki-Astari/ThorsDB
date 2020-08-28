#ifndef THORS_ANVIL_MYSQL_MYSQL_STREAM_H
#define THORS_ANVIL_MYSQL_MYSQL_STREAM_H

#include "ThorSQL/StreamSimple.h"

namespace ThorsAnvil::DB::MySQL
{

class MySQLStream: public SQL::StreamSimple
{
    public:
        MySQLStream(int fd)
            : SQL::StreamSimple(fd)
        {}
        MySQLStream(std::string const& host, int port = 3306)
            : SQL::StreamSimple(host, port ? port : 3306)
        {}
};

}

#endif
