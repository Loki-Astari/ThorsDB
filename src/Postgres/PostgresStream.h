#ifndef THORS_ANVIL_DB_POSTGRES_POSTGRES_STREAM_H
#define THORS_ANVIL_DB_POSTGRES_POSTGRES_STREAM_H

#include "PostgresConfig.h"
#include "ThorsDBCommon/StreamSimple.h"

namespace ThorsAnvil::DB::Postgres
{

class PostgresStream: public Common::StreamSimple
{
    public:
        PostgresStream(int fd)
            : Common::StreamSimple(fd)
        {}
        PostgresStream(std::string const& host, int port = 5432)
            : Common::StreamSimple(host, port ? port : 5432)
        {}
};

}

#endif
