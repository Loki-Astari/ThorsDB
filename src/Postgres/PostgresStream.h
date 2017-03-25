#ifndef THORS_ANVIL_PostG_POSTGRES_STREAM_H
#define THORS_ANVIL_PostG_POSTGRES_STREAM_H

#include "ThorSQL/StreamSimple.h"

namespace ThorsAnvil
{
    namespace Postgres
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
}

#endif
