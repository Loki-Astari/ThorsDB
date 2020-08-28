#ifndef THORS_ANVIL_DB_POSTGRESS_POSTGRES_STREAM_H
#define THORS_ANVIL_DB_POSTGRESS_POSTGRES_STREAM_H

#include "ThorSQL/StreamSimple.h"

namespace ThorsAnvil::DB::Postgres
{

class PostgresStream: public SQL::StreamSimple
{
    public:
        PostgresStream(int fd)
            : SQL::StreamSimple(fd)
        {}
        PostgresStream(std::string const& host, int port = 5432)
            : SQL::StreamSimple(host, port ? port : 5432)
        {}
};

}

#endif
